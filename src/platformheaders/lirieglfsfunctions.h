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

    typedef void (*EnableScreenCastType)(QScreen *screen);
    static QByteArray enableScreenCastIdentifier();
    static void enableScreenCast(QScreen *screen);

    typedef void (*DisableScreenCastType)(QScreen *screen);
    static QByteArray disableScreenCastIdentifier();
    static void disableScreenCast(QScreen *screen);
};

class LIRIPLATFORMHEADERS_EXPORT ScreenCastFrameEvent : public QEvent
{
public:
    explicit ScreenCastFrameEvent();

    QScreen *screen = nullptr;
    QPoint offset;
    QSize size;
    quint32 drmFormat = 0;
    quint64 modifier = 0;
    quint32 numObjects = 0;

    static QEvent::Type eventType;

    static QEvent::Type registeredType();
};

class LIRIPLATFORMHEADERS_EXPORT ScreenCastObjectEvent : public QEvent
{
public:
    explicit ScreenCastObjectEvent();

    QScreen *screen = nullptr;
    quint32 index = 0;
    int fd = -1;
    quint32 size = 0;
    quint32 offset = 0;
    quint32 stride = 0;
    quint32 planeIndex = 0;

    static QEvent::Type eventType;

    static QEvent::Type registeredType();
};

class LIRIPLATFORMHEADERS_EXPORT ScreenCastReadyEvent : public QEvent
{
public:
    explicit ScreenCastReadyEvent();

    QScreen *screen = nullptr;
    quint64 tv_sec = 0;
    quint32 tv_nsec = 0;

    static QEvent::Type eventType;

    static QEvent::Type registeredType();
};

} // namespace Platform

} // namespace Liri

#endif // LIRIEGLFSFUNCTIONS_H
