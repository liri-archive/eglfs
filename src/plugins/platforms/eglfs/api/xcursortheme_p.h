// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef XCURSORTHEME_H
#define XCURSORTHEME_H

#include <QImage>
#include <QMap>
#include <QString>

#include "qeglfsglobal_p.h"

namespace Liri {

namespace Platform {

class Q_EGLFS_EXPORT XcursorTheme
{
    friend class XcursorThemePrivate;
public:
    struct Cursor
    {
        bool isValid = false;
        QImage image;
        QPoint hotSpot;
    };

    XcursorTheme();
    ~XcursorTheme();

    void loadTheme(const QString &name, int size);

    bool isLoaded() const;

    QStringList cursorNames() const;
    Cursor cursorForName(const QString &name) const;
    Cursor cursorForShape(Qt::CursorShape shape) const;

private:
    QString m_name;
    int m_size = 0;
    QMap<QString, Cursor> m_cursors;
    bool m_loaded = false;

    void unloadTheme();
};

} // namespace Platform

} // namespace Liri

#endif // XCURSORTHEME_H
