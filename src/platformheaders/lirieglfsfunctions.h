/****************************************************************************
 * This file is part of Liri.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#ifndef LIRIELGFSFUNCTIONS_H
#define LIRIELGFSFUNCTIONS_H

#include <QEvent>
#include <QGuiApplication>
#include <QImage>

#include <LiriPlatformHeaders/liriplatformheadersglobal.h>

namespace Liri {

namespace Platform {

class LIRIPLATFORMHEADERS_EXPORT ScreenChange
{
public:
    explicit ScreenChange() = default;

    QScreen *screen = nullptr;
    bool enabled = false;
    QPoint position;
    QSize resolution;
    int refreshRate = 60;
    qreal scale = 1.0f;
};

class LIRIPLATFORMHEADERS_EXPORT EglFSFunctions
{
public:
    enum PowerState {
        PowerStateOn,
        PowerStateStandby,
        PowerStateSuspend,
        PowerStateOff
    };

    typedef PowerState (*GetPowerStateType)(QScreen *screen);
    static QByteArray getPowerStateIdentifier();
    static PowerState getPowerState(QScreen *screen);

    typedef void (*SetPowerStateType)(QScreen *screen, PowerState state);
    static QByteArray setPowerStateIdentifier();
    static void setPowerState(QScreen *screen, PowerState state);

    typedef bool (*TestScreenChangesType)(const QVector<ScreenChange> &changes);
    static QByteArray testScreenChangesIdentifier();
    static bool testScreenChanges(const QVector<ScreenChange> &changes);

    typedef bool (*ApplyScreenChangesType)(const QVector<ScreenChange> &changes);
    static QByteArray applyScreenChangesIdentifier();
    static bool applyScreenChanges(const QVector<ScreenChange> &changes);

    typedef void (*EnableScreenCaptureType)(QScreen *screen);
    static QByteArray enableScreenCaptureIdentifier();
    static void enableScreenCapture(QScreen *screen);

    typedef void (*DisableScreenCaptureType)(QScreen *screen);
    static QByteArray disableScreenCaptureIdentifier();
    static void disableScreenCapture(QScreen *screen);
};

class LIRIPLATFORMHEADERS_EXPORT FrameCaptureEvent : public QEvent
{
public:
    FrameCaptureEvent(const QImage &image);

    QImage capture;

    static QEvent::Type eventType;

    static QEvent::Type registeredType();
};

} // namespace Platform

} // namespace Liri

#endif // LIRIEGLFSFUNCTIONS_H
