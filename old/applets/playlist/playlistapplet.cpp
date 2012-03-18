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
#include "playlistapplet.h"
#include "playlistwidget.h"

#include <QGraphicsLinearLayout>

#include <QWidget>

#include <KConfigDialog>
#include <KUrl>
#include <KDebug>

#include <Plasma/Theme>

PlaylistApplet::PlaylistApplet(QObject *parent, const QVariantList &args)
    : MediaCenter::Playlist(parent, args),
    m_svg(new Plasma::FrameSvg(this)),
    m_playlistWidget(0)
{
    setAspectRatioMode(Plasma::IgnoreAspectRatio);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);

    setAcceptDrops(true);
    setHasConfigurationInterface(true);
}

PlaylistApplet::~PlaylistApplet()
{}

void PlaylistApplet::constraintsEvent(Plasma::Constraints constraints)
{
    Q_UNUSED(constraints);
    setBackgroundHints(Plasma::Applet::NoBackground);
    setMarginsFromTheme();
}

void PlaylistApplet::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event)

    m_svg->resizeFrame(rect().size());
}

void PlaylistApplet::paintInterface(QPainter *painter, const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
    m_svg->paintFrame(painter, option->rect);
}

void PlaylistApplet::init()
{
    m_svg->setEnabledBorders(Plasma::FrameSvg::LeftBorder);
    m_svg->setCacheAllRenderedFrames(true);
    m_svg->setImagePath(Plasma::Theme::defaultTheme()->imagePath("widgets/background"));

    setMarginsFromTheme();
    connect (Plasma::Theme::defaultTheme(), SIGNAL(themeChanged()), this, SLOT(slotThemeChanged()));

    // we make sure the widget is constructed
    m_playlistWidget = new PlaylistWidget;
    connect (m_playlistWidget, SIGNAL(mediaActivated(const MediaCenter::Media &)), this, SIGNAL(mediaActivated(const MediaCenter::Media&)));
    connect (m_playlistWidget, SIGNAL(mediasAppended(QList<MediaCenter::Media>)), this, SIGNAL(mediasAppended(QList<MediaCenter::Media>)));

    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout;
    layout->addItem(m_playlistWidget);
    setLayout(layout);

    loadConfiguration();
}

void PlaylistApplet::setMarginsFromTheme()
{
    qreal left;
    qreal right;
    qreal top;
    qreal bottom;

    m_svg->getMargins(left, top, right, bottom);
    //kDebug() << "setting to" << left << top << right << bottom;
    setContentsMargins(left, top, right, bottom);
}

void PlaylistApplet::slotThemeChanged()
{
    m_svg->clearCache();
    m_svg->setImagePath(Plasma::Theme::defaultTheme()->imagePath("widgets/background"));
    setMarginsFromTheme();
}

int PlaylistApplet::length()
{
    return m_playlistWidget->length();
}

QList<MediaCenter::Media> PlaylistApplet::medias(MediaCenter::MediaType type)
{
    return m_playlistWidget->medias(type);
}

void PlaylistApplet::createConfigurationInterface(KConfigDialog *parent)
{
    QWidget *config = new QWidget(parent);
    configUi.setupUi(config);
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));

    configUi.multiplePlaylistsCheckBox->setChecked(m_multiplePlaylists);

    parent->addPage(config, i18n("Playlist settings"), icon());

    connect (parent, SIGNAL(accepted()), this, SLOT(configAccepted()));
    connect (configUi.multiplePlaylistsCheckBox, SIGNAL(toggled(bool)), parent, SLOT(settingsModified()));
}

void PlaylistApplet::configAccepted()
{
    KConfigGroup cf = config();

    m_multiplePlaylists = configUi.multiplePlaylistsCheckBox->isChecked();
    cf.writeEntry("MultiplePlaylists", m_multiplePlaylists);
    m_playlistWidget->setMultiplePlaylistsEnabled(m_multiplePlaylists);
}

void PlaylistApplet::loadConfiguration()
{
    KConfigGroup cf = config();

    m_multiplePlaylists = cf.readEntry<bool>("MultiplePlaylists", false);
    m_playlistWidget->setMultiplePlaylistsEnabled(m_multiplePlaylists);
}

void PlaylistApplet::appendMedia(const QList<MediaCenter::Media> &medias, const QString &playlist)
{
    Q_UNUSED(playlist); // TODO: take care of the playlist specification

    QList<QUrl> urls;
    foreach (const MediaCenter::Media &media, medias) {
        if (media.first == MediaCenter::Invalid) {
            continue;
        }
        urls << KUrl(media.second);
    }
    m_playlistWidget->append(urls);
}

void PlaylistApplet::setPlaylistMediaType(const MediaCenter::MediaType &type)
{
    m_playlistWidget->setMediaTypeToShow(type);
}


K_EXPORT_PLASMA_APPLET(playlist, PlaylistApplet)
