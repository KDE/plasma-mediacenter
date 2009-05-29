/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/
#include "mediabrowser.h"
#include "abstractmediaitemview.h"

#include <QWidget>
#include <QGraphicsLinearLayout>

MediaBrowser::MediaBrowser(QObject *parent, const QVariantList &args)
    : Plasma::Applet(parent, args)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    layout->addItem(new AbstractMediaItemView(this));
    setLayout(layout);
}

MediaBrowser::~MediaBrowser()
{}

void MediaBrowser::init()
{
}

K_EXPORT_PLASMA_APPLET(mediabrowser, MediaBrowser)
