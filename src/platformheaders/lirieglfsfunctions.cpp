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

#include <QByteArray>

#include "lirieglfsfunctions.h"

namespace Liri {

namespace Platform {

QByteArray EglFSFunctions::setCursorThemeIdentifier()
{
    return QByteArrayLiteral("LiriEglFSSetCursorTheme");
}

void EglFSFunctions::setCursorTheme(const QString &name, int size)
{
    SetCursorThemeType func = reinterpret_cast<SetCursorThemeType>(QGuiApplication::platformFunction(setCursorThemeIdentifier()));
    if (func)
        func(name, size);
}

QByteArray EglFSFunctions::getPowerStateIdentifier()
{
    return QByteArrayLiteral("LiriEglFSGetPowerState");
}

EglFSFunctions::PowerState EglFSFunctions::getPowerState(QScreen *screen)
{
    GetPowerStateType func = reinterpret_cast<GetPowerStateType>(QGuiApplication::platformFunction(getPowerStateIdentifier()));
    if (func)
        return func(screen);
    return PowerStateOn;
}

QByteArray EglFSFunctions::setPowerStateIdentifier()
{
    return QByteArrayLiteral("LiriEglFSSetPowerState");
}

void EglFSFunctions::setPowerState(QScreen *screen, PowerState state)
{
    SetPowerStateType func = reinterpret_cast<SetPowerStateType>(QGuiApplication::platformFunction(setPowerStateIdentifier()));
    if (func)
        func(screen, state);
}

QByteArray EglFSFunctions::testScreenChangesIdentifier()
{
    return "LiriEglFSTestScreenChanges";
}

bool EglFSFunctions::testScreenChanges(const QVector<ScreenChange> &changes)
{
    TestScreenChangesType func = reinterpret_cast<TestScreenChangesType>(QGuiApplication::platformFunction(testScreenChangesIdentifier()));
    if (func)
        return func(changes);
    return false;
}

QByteArray EglFSFunctions::applyScreenChangesIdentifier()
{
    return "LiriEglFSApplyScreenChanges";
}

bool EglFSFunctions::applyScreenChanges(const QVector<ScreenChange> &changes)
{
    ApplyScreenChangesType func = reinterpret_cast<ApplyScreenChangesType>(QGuiApplication::platformFunction(applyScreenChangesIdentifier()));
    if (func)
        return func(changes);
    return false;
}

QByteArray EglFSFunctions::enableScreenCastIdentifier()
{
    return QByteArrayLiteral("LiriEglFSEnableScreenCast");
}

void EglFSFunctions::enableScreenCast(QScreen *screen)
{
    EnableScreenCastType func = reinterpret_cast<EnableScreenCastType>(QGuiApplication::platformFunction(enableScreenCastIdentifier()));
    if (func)
        func(screen);
}

QByteArray EglFSFunctions::disableScreenCastIdentifier()
{
    return QByteArrayLiteral("LiriEglFSDisableStreaming");
}

void EglFSFunctions::disableScreenCast(QScreen *screen)
{
    DisableScreenCastType func = reinterpret_cast<DisableScreenCastType>(QGuiApplication::platformFunction(disableScreenCastIdentifier()));
    if (func)
        func(screen);
}

/*
 * Screencast
 */

QEvent::Type ScreenCastFrameEvent::eventType = QEvent::None;

ScreenCastFrameEvent::ScreenCastFrameEvent()
    : QEvent(registeredType())
{
}

QEvent::Type ScreenCastFrameEvent::registeredType()
{
    if (eventType == QEvent::None) {
        int generatedType = QEvent::registerEventType();
        eventType = static_cast<QEvent::Type>(generatedType);
    }

    return eventType;
}


QEvent::Type ScreenCastObjectEvent::eventType = QEvent::None;

ScreenCastObjectEvent::ScreenCastObjectEvent()
    : QEvent(registeredType())
{
}

QEvent::Type ScreenCastObjectEvent::registeredType()
{
    if (eventType == QEvent::None) {
        int generatedType = QEvent::registerEventType();
        eventType = static_cast<QEvent::Type>(generatedType);
    }

    return eventType;
}


QEvent::Type ScreenCastReadyEvent::eventType = QEvent::None;

ScreenCastReadyEvent::ScreenCastReadyEvent()
    : QEvent(registeredType())
{
}

QEvent::Type ScreenCastReadyEvent::registeredType()
{
    if (eventType == QEvent::None) {
        int generatedType = QEvent::registerEventType();
        eventType = static_cast<QEvent::Type>(generatedType);
    }

    return eventType;
}

} // namespace Platform

} // namespace Liri
