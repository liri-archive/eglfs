/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Copyright (C) 2015 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qeglfskmsgbmscreen.h"
#include "qeglfskmsgbmdevice.h"
#include "qeglfskmsgbmcursor.h"
#include "qeglfsintegration_p.h"

#include <QtCore/QLoggingCategory>
#include <QOpenGLFunctions_3_0>
#include <QtGui/private/qguiapplication_p.h>
#include <QtFbSupport/private/qfbvthandler_p.h>

#include <LiriLogind/LiriLogind>

#include <errno.h>

static const char *formatGLError(GLenum err)
{
    switch(err) {
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    default:
        return (QLatin1String("0x") + QString::number(err, 16)).toLocal8Bit().constData();
    }
}

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(qLcEglfsKmsDebug)

static inline uint32_t drmFormatToGbmFormat(uint32_t drmFormat)
{
    Q_ASSERT(DRM_FORMAT_XRGB8888 == GBM_FORMAT_XRGB8888);
    return drmFormat;
}

static inline uint32_t gbmFormatToDrmFormat(uint32_t gbmFormat)
{
    Q_ASSERT(DRM_FORMAT_XRGB8888 == GBM_FORMAT_XRGB8888);
    return gbmFormat;
}

void QEglFSKmsGbmScreen::bufferDestroyedHandler(gbm_bo *bo, void *data)
{
    FrameBuffer *fb = static_cast<FrameBuffer *>(data);

    if (fb->fb) {
        gbm_device *device = gbm_bo_get_device(bo);
        drmModeRmFB(gbm_device_get_fd(device), fb->fb);
    }

    delete fb;
}

QEglFSKmsGbmScreen::FrameBuffer *QEglFSKmsGbmScreen::framebufferForBufferObject(gbm_bo *bo)
{
    {
        FrameBuffer *fb = static_cast<FrameBuffer *>(gbm_bo_get_user_data(bo));
        if (fb)
            return fb;
    }

    uint32_t width = gbm_bo_get_width(bo);
    uint32_t height = gbm_bo_get_height(bo);
    uint32_t handles[4] = { gbm_bo_get_handle(bo).u32 };
    uint32_t strides[4] = { gbm_bo_get_stride(bo) };
    uint32_t offsets[4] = { 0 };
    uint32_t pixelFormat = gbmFormatToDrmFormat(gbm_bo_get_format(bo));

    QScopedPointer<FrameBuffer> fb(new FrameBuffer);
    qCDebug(qLcEglfsKmsDebug, "Adding FB, size %ux%u, DRM format 0x%x", width, height, pixelFormat);

    int ret = drmModeAddFB2(device()->fd(), width, height, pixelFormat,
                            handles, strides, offsets, &fb->fb, 0);

    if (ret) {
        qWarning("Failed to create KMS FB!");
        return nullptr;
    }

    gbm_bo_set_user_data(bo, fb.data(), bufferDestroyedHandler);
    return fb.take();
}

QEglFSKmsGbmScreen::QEglFSKmsGbmScreen(QKmsDevice *device, const QKmsOutput &output, bool headless)
    : QEglFSKmsScreen(device, output, headless)
    , m_gbm_surface(nullptr)
    , m_gbm_bo_current(nullptr)
    , m_gbm_bo_next(nullptr)
    , m_flipPending(false)
    , m_cursor(nullptr)
    , m_cloneSource(nullptr)
{
}

QEglFSKmsGbmScreen::~QEglFSKmsGbmScreen()
{
    const int remainingScreenCount = qGuiApp->screens().count();
    qCDebug(qLcEglfsKmsDebug, "Screen dtor. Remaining screens: %d", remainingScreenCount);
    if (!remainingScreenCount && !device()->screenConfig()->separateScreens())
        static_cast<QEglFSKmsGbmDevice *>(device())->destroyGlobalCursor();
}

QPlatformCursor *QEglFSKmsGbmScreen::cursor() const
{
    QKmsScreenConfig *config = device()->screenConfig();
    if (config->headless())
        return nullptr;
    if (config->hwCursor()) {
        if (!config->separateScreens())
            return static_cast<QEglFSKmsGbmDevice *>(device())->globalCursor();

        if (m_cursor.isNull()) {
            QEglFSKmsGbmScreen *that = const_cast<QEglFSKmsGbmScreen *>(this);
            that->m_cursor.reset(new QEglFSKmsGbmCursor(that));
        }

        return m_cursor.data();
    } else {
        return QEglFSScreen::cursor();
    }
}

gbm_surface *QEglFSKmsGbmScreen::createSurface()
{
    if (!m_gbm_surface) {
        uint32_t gbmFormat = drmFormatToGbmFormat(m_output.drm_format);
        qCDebug(qLcEglfsKmsDebug, "Creating gbm_surface for screen %s with format 0x%x", qPrintable(name()), gbmFormat);
        m_gbm_surface = gbm_surface_create(static_cast<QEglFSKmsGbmDevice *>(device())->gbmDevice(),
                                           rawGeometry().width(),
                                           rawGeometry().height(),
                                           gbmFormat,
                                           GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);
    }
    return m_gbm_surface; // not owned, gets destroyed in QEglFSKmsGbmIntegration::destroyNativeWindow() via QEglFSKmsGbmWindow::invalidateSurface()
}

void QEglFSKmsGbmScreen::resetSurface()
{
    m_gbm_surface = nullptr;
}

void QEglFSKmsGbmScreen::setSurface(gbm_surface *surface)
{
    m_gbm_surface = surface;
}

void QEglFSKmsGbmScreen::initCloning(QPlatformScreen *screenThisScreenClones,
                                     const QVector<QPlatformScreen *> &screensCloningThisScreen)
{
    // clone destinations need to know the clone source
    const bool clonesAnother = screenThisScreenClones != nullptr;
    if (clonesAnother && !screensCloningThisScreen.isEmpty()) {
        qWarning("QEglFSKmsGbmScreen %s cannot be clone source and destination at the same time", qPrintable(name()));
        return;
    }
    if (clonesAnother)
        m_cloneSource = static_cast<QEglFSKmsGbmScreen *>(screenThisScreenClones);

    // clone sources need to know their additional destinations
    for (QPlatformScreen *s : screensCloningThisScreen) {
        CloneDestination d;
        d.screen = static_cast<QEglFSKmsGbmScreen *>(s);
        m_cloneDests.append(d);
    }
}

void QEglFSKmsGbmScreen::ensureModeSet(uint32_t fb)
{
    QKmsOutput &op(output());
    const int fd = device()->fd();

    if (!op.mode_set) {
        op.mode_set = true;

        bool doModeSet = true;
        drmModeCrtcPtr currentMode = drmModeGetCrtc(fd, op.crtc_id);
        const bool alreadySet = currentMode && !memcmp(&currentMode->mode, &op.modes[op.mode], sizeof(drmModeModeInfo));
        if (currentMode)
            drmModeFreeCrtc(currentMode);
        if (alreadySet) {
            static bool alwaysDoSet = qEnvironmentVariableIntValue("QT_QPA_EGLFS_ALWAYS_SET_MODE");
            if (!alwaysDoSet) {
                qCDebug(qLcEglfsKmsDebug, "Mode already set, skipping modesetting for screen %s", qPrintable(name()));
                doModeSet = false;
            }
        }

        if (doModeSet) {
            qCDebug(qLcEglfsKmsDebug, "Setting mode for screen %s", qPrintable(name()));
            int ret = drmModeSetCrtc(fd,
                                     op.crtc_id,
                                     fb,
                                     0, 0,
                                     &op.connector_id, 1,
                                     &op.modes[op.mode]);

            if (ret == 0)
                setPowerState(PowerStateOn);
            else
                qErrnoWarning(errno, "Could not set DRM mode for screen %s", qPrintable(name()));
        }
    }
}

void QEglFSKmsGbmScreen::waitForFlip()
{
    if (m_headless || m_cloneSource || modeChangeRequested())
        return;

    // Avoid permission denied error when session is not active
    if (!Liri::Logind::instance()->isSessionActive())
        return;

    // Don't lock the mutex unless we actually need to
    if (!m_gbm_bo_next)
        return;

    QMutexLocker lock(&m_waitForFlipMutex);
    while (m_gbm_bo_next) {
        drmEventContext drmEvent;
        memset(&drmEvent, 0, sizeof(drmEvent));
        drmEvent.version = 2;
        drmEvent.vblank_handler = nullptr;
        drmEvent.page_flip_handler = pageFlipHandler;
        drmHandleEvent(device()->fd(), &drmEvent);
    }
}

void QEglFSKmsGbmScreen::flip()
{
    // For headless screen just return silently. It is not necessarily an error
    // to end up here, so show no warnings.
    if (m_headless)
        return;

    if (m_cloneSource) {
        qWarning("Screen %s clones another screen. swapBuffers() not allowed.", qPrintable(name()));
        return;
    }

    if (modeChangeRequested())
        return;

    if (!m_gbm_surface) {
        qWarning("Cannot sync before platform init!");
        return;
    }

    m_gbm_bo_next = gbm_surface_lock_front_buffer(m_gbm_surface);
    if (!m_gbm_bo_next) {
        qWarning("Could not lock GBM surface front buffer!");
        return;
    }

    // GBM surface is released on page flip so we should record
    // the frame here, before it's presented
    if (isRecordingEnabled())
        recordFrame(m_gbm_bo_next, gbm_bo_get_width(m_gbm_bo_next),
                    gbm_bo_get_height(m_gbm_bo_next));

    FrameBuffer *fb = framebufferForBufferObject(m_gbm_bo_next);
    ensureModeSet(fb->fb);

    QKmsOutput &op(output());
    const int fd = device()->fd();
    m_flipPending = true;
    int ret = drmModePageFlip(fd,
                              op.crtc_id,
                              fb->fb,
                              DRM_MODE_PAGE_FLIP_EVENT,
                              this);
    if (ret) {
        qErrnoWarning("Could not queue DRM page flip on screen %s", qPrintable(name()));
        m_flipPending = false;
        gbm_surface_release_buffer(m_gbm_surface, m_gbm_bo_next);
        m_gbm_bo_next = nullptr;
        return;
    }

    for (CloneDestination &d : m_cloneDests) {
        if (d.screen != this) {
            d.screen->ensureModeSet(fb->fb);
            d.cloneFlipPending = true;
            int ret = drmModePageFlip(fd,
                                      d.screen->output().crtc_id,
                                      fb->fb,
                                      DRM_MODE_PAGE_FLIP_EVENT,
                                      d.screen);
            if (ret) {
                qErrnoWarning("Could not queue DRM page flip for clone screen %s", qPrintable(name()));
                d.cloneFlipPending = false;
            }
        }
    }
}

void QEglFSKmsGbmScreen::recordFrame(EGLClientBuffer bo, int width, int height)
{
    // Create EGL image from BO
    EGLImage image = eglCreateImage(display(), nullptr, EGL_NATIVE_PIXMAP_KHR, bo, nullptr);
    if (image == EGL_NO_IMAGE_KHR) {
        qCritical("Error creating EGLImage: %s", formatGLError(glGetError()));
        return;
    }

    // Create GL 2D texture for framebuffer
    GLuint texture;
    glGenTextures(1, &texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, texture);
    PFNGLEGLIMAGETARGETTEXTURE2DOESPROC imageTexture2D =
        reinterpret_cast<PFNGLEGLIMAGETARGETTEXTURE2DOESPROC>(eglGetProcAddress("glEGLImageTargetTexture2DOES"));
    if (imageTexture2D)
        imageTexture2D(GL_TEXTURE_2D, image);
    else
        qCritical("No glEGLImageTargetTexture2DOES function available");

    // OpenGL 3.0 functions
    auto glContext = QOpenGLContext::currentContext();
    auto funcs = glContext->versionFunctions<QOpenGLFunctions_3_0>();

    // Bind framebuffer to copy pixels from
    GLuint framebuffer;
    funcs->glGenFramebuffers(1, &framebuffer);
    funcs->glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    funcs->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
    const GLenum status = funcs->glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        qCritical("glCheckFramebufferStatus failed: %s", formatGLError(glGetError()));
        funcs->glDeleteTextures(1, &texture);
        funcs->glDeleteFramebuffers(1, &framebuffer);
        eglDestroyImage(display(), image);
        return;
    }

    // Capture to an image
    QImage capture(QSize(width, height), QImage::Format_RGBA8888);
    glViewport(0, 0, width, height);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, capture.bits());

    // Post the event
    QCoreApplication::postEvent(QCoreApplication::instance(),
                                new Liri::Platform::FrameCaptureEvent(capture));

    // Free resources
    funcs->glDeleteTextures(1, &texture);
    funcs->glDeleteFramebuffers(1, &framebuffer);
    eglDestroyImage(display(), image);
}

void QEglFSKmsGbmScreen::pageFlipHandler(int fd, unsigned int sequence, unsigned int tv_sec, unsigned int tv_usec, void *user_data)
{
    Q_UNUSED(fd);
    Q_UNUSED(sequence);
    Q_UNUSED(tv_sec);
    Q_UNUSED(tv_usec);

    QEglFSKmsGbmScreen *screen = static_cast<QEglFSKmsGbmScreen *>(user_data);
    screen->flipFinished();
}

void QEglFSKmsGbmScreen::flipFinished()
{
    if (m_cloneSource) {
        m_cloneSource->cloneDestFlipFinished(this);
        return;
    }

    m_flipPending = false;
    updateFlipStatus();
}

void QEglFSKmsGbmScreen::cloneDestFlipFinished(QEglFSKmsGbmScreen *cloneDestScreen)
{
    for (CloneDestination &d : m_cloneDests) {
        if (d.screen == cloneDestScreen) {
            d.cloneFlipPending = false;
            break;
        }
    }
    updateFlipStatus();
}

void QEglFSKmsGbmScreen::updateFlipStatus()
{
    Q_ASSERT(!m_cloneSource);

    if (m_flipPending)
        return;

    for (const CloneDestination &d : m_cloneDests) {
        if (d.cloneFlipPending)
            return;
    }

    if (m_gbm_bo_current)
        gbm_surface_release_buffer(m_gbm_surface,
                                   m_gbm_bo_current);

    m_gbm_bo_current = m_gbm_bo_next;
    m_gbm_bo_next = nullptr;
}

QT_END_NAMESPACE
