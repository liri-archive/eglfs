// SPDX-FileCopyrightText: 2021 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "xcursortheme_p.h"
extern "C" {
#include "xcursor.h"
}

namespace Liri {

namespace Platform {

class XcursorThemePrivate
{
public:
    static void load_callback(XcursorImages *images, void *user_data)
    {
        auto *theme = static_cast<XcursorTheme *>(user_data);
        auto name = QString::fromUtf8(images->name);

        // Do not load a cursor if it's already present
        if (theme->m_cursors.contains(name)) {
            XcursorImagesDestroy(images);
            return;
        }

        // Convert only first item of XcursorImages to a QImage, because we won't support animated cursors
        QImage image(images->images[0]->width, images->images[0]->height, QImage::Format_ARGB32);
        memcpy(image.bits(), images->images[0]->pixels, images->images[0]->width * images->images[0]->height * 4);

        // Hotspot
        QPoint hotspot(images->images[0]->xhot, images->images[0]->yhot);

        // Save the cursor
        theme->m_cursors[name] = XcursorTheme::Cursor{true, image, hotspot};

        XcursorImagesDestroy(images);
    }
};

XcursorTheme::XcursorTheme()
{
}

XcursorTheme::~XcursorTheme()
{
    unloadTheme();
}

void XcursorTheme::loadTheme(const QString &name, int size)
{
    if (m_name == name && m_size == size)
        return;

    if (m_loaded)
        unloadTheme();

    m_name = name.isEmpty() ? QStringLiteral("default") : name;
    m_size = size;
    xcursor_load_theme(qPrintable(m_name), m_size, XcursorThemePrivate::load_callback, this);
    m_loaded = true;
}

bool XcursorTheme::isLoaded() const
{
    return m_loaded;
}

QStringList XcursorTheme::cursorNames() const
{
    return m_cursors.keys();
}

XcursorTheme::Cursor XcursorTheme::cursorForName(const QString &name) const
{
    if (!m_loaded)
        return XcursorTheme::Cursor();
    return m_cursors.value(name);
}

XcursorTheme::Cursor XcursorTheme::cursorForShape(Qt::CursorShape shape) const
{
    if (!m_loaded)
        return XcursorTheme::Cursor();

    static constexpr struct ShapeMap {
        Qt::CursorShape shape;
        const char name[50];
    } cursorShapeMap[] = {
        {Qt::ArrowCursor, "left_ptr"},
        {Qt::ArrowCursor, "default"},
        {Qt::ArrowCursor, "top_left_arrow"},
        {Qt::ArrowCursor, "left_arrow"},

        {Qt::UpArrowCursor, "up_arrow"},

        {Qt::CrossCursor, "cross"},

        {Qt::WaitCursor, "wait"},
        {Qt::WaitCursor, "watch"},
        {Qt::WaitCursor, "0426c94ea35c87780ff01dc239897213"},

        {Qt::IBeamCursor, "ibeam"},
        {Qt::IBeamCursor, "text"},
        {Qt::IBeamCursor, "xterm"},

        {Qt::SizeVerCursor, "size_ver"},
        {Qt::SizeVerCursor, "ns-resize"},
        {Qt::SizeVerCursor, "v_double_arrow"},
        {Qt::SizeVerCursor, "00008160000006810000408080010102"},

        {Qt::SizeHorCursor, "size_hor"},
        {Qt::SizeHorCursor, "ew-resize"},
        {Qt::SizeHorCursor, "h_double_arrow"},
        {Qt::SizeHorCursor, "028006030e0e7ebffc7f7070c0600140"},

        {Qt::SizeBDiagCursor, "size_bdiag"},
        {Qt::SizeBDiagCursor, "nesw-resize"},
        {Qt::SizeBDiagCursor, "50585d75b494802d0151028115016902"},
        {Qt::SizeBDiagCursor, "fcf1c3c7cd4491d801f1e1c78f100000"},

        {Qt::SizeFDiagCursor, "size_fdiag"},
        {Qt::SizeFDiagCursor, "nwse-resize"},
        {Qt::SizeFDiagCursor, "38c5dff7c7b8962045400281044508d2"},
        {Qt::SizeFDiagCursor, "c7088f0f3e6c8088236ef8e1e3e70000"},

        {Qt::SizeAllCursor, "size_all"},

        {Qt::SplitVCursor, "split_v"},
        {Qt::SplitVCursor, "row-resize"},
        {Qt::SplitVCursor, "sb_v_double_arrow"},
        {Qt::SplitVCursor, "2870a09082c103050810ffdffffe0204"},
        {Qt::SplitVCursor, "c07385c7190e701020ff7ffffd08103c"},

        {Qt::SplitHCursor, "split_h"},
        {Qt::SplitHCursor, "col-resize"},
        {Qt::SplitHCursor, "sb_h_double_arrow"},
        {Qt::SplitHCursor, "043a9f68147c53184671403ffa811cc5"},
        {Qt::SplitHCursor, "14fef782d02440884392942c11205230"},

        {Qt::PointingHandCursor, "pointing_hand"},
        {Qt::PointingHandCursor, "pointer"},
        {Qt::PointingHandCursor, "hand1"},
        {Qt::PointingHandCursor, "e29285e634086352946a0e7090d73106"},

        {Qt::ForbiddenCursor, "forbidden"},
        {Qt::ForbiddenCursor, "not-allowed"},
        {Qt::ForbiddenCursor, "crossed_circle"},
        {Qt::ForbiddenCursor, "circle"},
        {Qt::ForbiddenCursor, "03b6e0fcb3499374a867c041f52298f0"},

        {Qt::WhatsThisCursor, "whats_this"},
        {Qt::WhatsThisCursor, "help"},
        {Qt::WhatsThisCursor, "question_arrow"},
        {Qt::WhatsThisCursor, "5c6cd98b3f3ebcb1f9c7f1c204630408"},
        {Qt::WhatsThisCursor, "d9ce0ab605698f320427677b458ad60b"},

        {Qt::BusyCursor, "left_ptr_watch"},
        {Qt::BusyCursor, "half-busy"},
        {Qt::BusyCursor, "progress"},
        {Qt::BusyCursor, "00000000000000020006000e7e9ffc3f"},
        {Qt::BusyCursor, "08e8e1c95fe2fc01f976f1e063a24ccd"},

        {Qt::OpenHandCursor, "openhand"},
        {Qt::OpenHandCursor, "fleur"},
        {Qt::OpenHandCursor, "5aca4d189052212118709018842178c0"},
        {Qt::OpenHandCursor, "9d800788f1b08800ae810202380a0822"},

        {Qt::ClosedHandCursor, "closedhand"},
        {Qt::ClosedHandCursor, "grabbing"},
        {Qt::ClosedHandCursor, "208530c400c041818281048008011002"},

        {Qt::DragCopyCursor, "dnd-copy"},
        {Qt::DragCopyCursor, "copy"},

        {Qt::DragMoveCursor, "dnd-move"},
        {Qt::DragMoveCursor, "move"},

        {Qt::DragLinkCursor, "dnd-link"},
        {Qt::DragLinkCursor, "link"},
    };

    const auto compareByShape = [](ShapeMap lhs, ShapeMap rhs) {
        return lhs.shape < rhs.shape;
    };

    const auto p = std::equal_range(std::begin(cursorShapeMap), std::end(cursorShapeMap),
                                    ShapeMap{shape, ""}, compareByShape);
    for (auto it = p.first; it != p.second; ++it) {
        const auto name = QString::fromLocal8Bit(it->name);
        if (m_cursors.contains(name))
            return m_cursors.value(name);
    }

    return XcursorTheme::Cursor();
}

void XcursorTheme::unloadTheme()
{
    m_cursors.clear();
    m_loaded = false;
}

} // namespace Platform

} // namespace Liri
