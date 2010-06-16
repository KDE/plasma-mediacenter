/***************************************************************************
 *   Copyright 2010 by Christophe Olinger <olingerc@binarylooks.com>       *
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

#ifndef PICTURESTATE_H
#define PICTURESTATE_H

#include "mediacenterstate.h"

#include <Plasma/IconWidget>
#include <Plasma/Slider>

namespace Nepomuk {
    class Resource;
}

class KRatingWidget;


namespace MediaCenter {

class MEDIACENTER_EXPORT PictureState : public MediaCenterState
{
    Q_OBJECT
public:
    PictureState(QState *parent = 0);
    virtual ~PictureState();

    virtual QList<QGraphicsWidget*> subComponents() const;
    virtual void configure();
    virtual void initConnections();

protected:
    virtual void onExit(QEvent* event);
    virtual void onEntry(QEvent* event);

private:
    void updateInfoDisplay();

    Plasma::IconWidget *m_pictureToBrowser;
    Plasma::IconWidget *m_startSlideshow;
    Plasma::Slider *m_slideshowTimeSlider;
    Plasma::IconWidget *m_slideshowLabel;
    Plasma::IconWidget *m_nextPicture;
    Plasma::IconWidget *m_previousPicture;
    Plasma::IconWidget *m_selectedMediasLabel;
    Plasma::IconWidget *m_currentlyPlayingLabel;
    Plasma::IconWidget *m_switchDisplayMode;
    MediaCenter::Media m_pictureMedia;

    KUrl m_lastDirectory;

    KRatingWidget *m_ratingWidget;
    QGraphicsProxyWidget *m_ratingProxyWidget;
    bool m_nepomuk;

private slots:
    void enterBrowsingState();
    void enterSinglePictureFullscreenState();
    void enterPictureFloatingState();
    void enterSlideshowState();

    void updateSLideshowInterval(const int &time);
    void onPlaybackStateChanged(const MediaCenter::PlaybackState &state);
    void setMedia(const MediaCenter::Media &media);

    void selectedMediasChanged(const QList<MediaCenter::Media> &list);
    void slotRatingChanged(const int rating);

    void pauseOnSkip();
    void toggleFloatingState();

};

} //namespace end
#endif // PICTURESTATE_H
