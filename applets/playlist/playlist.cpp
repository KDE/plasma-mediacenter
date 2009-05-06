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
#include "playlist.h"
#include "playlistwidget.h"

#include <QWidget>
#include <KConfigDialog>

Playlist::Playlist(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args), m_playlistWidget(0)
{
    setPopupIcon("view-media-playlist");
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setAcceptDrops(true);
    setHasConfigurationInterface(true);

    // we make sure the widget is constructed
    (void)graphicsWidget();
}

Playlist::~Playlist()
{}

QGraphicsWidget* Playlist::graphicsWidget()
{
    if (!m_playlistWidget) {
        m_playlistWidget = new PlaylistWidget(this);
    }

    return m_playlistWidget;
}

void Playlist::init()
{
    loadConfiguration();
}

void Playlist::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *config = new QWidget(parent);
    configUi.setupUi(config);

    configUi.multiplePlaylistsCheckBox->setChecked(m_multiplePlaylists);

    parent->addPage(config, i18n("General"), icon());

    connect (parent, SIGNAL(accepted()), this, SLOT(configAccepted()));
}

void Playlist::configAccepted()
{
    KConfigGroup cf = config();

    m_multiplePlaylists = configUi.multiplePlaylistsCheckBox->isChecked();
    cf.writeEntry("MultiplePlaylists", m_multiplePlaylists);
    m_playlistWidget->setMultiplePlaylistsEnabled(m_multiplePlaylists);
}

void Playlist::loadConfiguration()
{
    KConfigGroup cf = config();

    m_multiplePlaylists = cf.readEntry<bool>("MultiplePlaylists", false);
    m_playlistWidget->setMultiplePlaylistsEnabled(m_multiplePlaylists);
}

K_EXPORT_PLASMA_APPLET(playlist, Playlist)

#include "playlist.moc"
