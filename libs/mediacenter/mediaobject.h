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
#ifndef MEDIAOBJECT_H
#define MEDIAOBJECT_H

#include <QObject>
#include <mediacenter.h>

class KUrl;

namespace Phonon {
    class MediaObject;
}

namespace MediaCenter {

class MediaSource;

/**
 * @class MediaObject mediaobject.h
 * @brief Basic reproduction class for Media Center Type medias.
 *
 * This class actually acts as a wrapper for Phonon::MediaObject giving
 * the chance to also include in the queue other media types like pictures.
 * It does not inherit directly from Phonon::MediaObject in order to keep compatibility
 * with other Phonon classes. Use phononMediaObject() to retrieve the Phonon::MediaObject object
 * and take care of connecting to the signals from this implementation in order to handle
 * features like picture slideshow.
 */

class MediaObject : public QObject
{
    Q_OBJECT
public:
    ~MediaObject();

    MediaObject *self();

    /**
     * @return the current media type playing/showed.
     */
    MediaType currentType() const;

    /**
     * @return the url of the current media or an enoty KUrl
     * if the current MediaType is a Dvd/Vcd/Cd
     */
    KUrl currentMedia() const;

    /**
     * @return the current playing device name if the current MediaTypee
     * is a Dvd/Vcd/Cd
     */
    QString currentDeviceName() const;

    /**
     * @return the current source.
     */
    MediaSource currentSource() const;

    /**
     * Use this method in order to retrieve the
     * Phonon::MediaObject associated with the reproduction.
     *
     * @note: Note that since Phonon does not handle
     * Picture media type you'll always want to connect
     * to signals of this implementation like mediaSourceChanged()
     * instead of calling directly the ones from Phonon::MediaObject.
     * This way you'll get notified when the audio/video reproduction stops
     * in order to allow the slideshow of pictures in the queue.
     */
    Phonon::MediaObject *phononMediaObject();

signals:
    void currentSourceChanged(MediaType, const MediaCenter::MediaSource &source);

private:
    MediaObject(QObject *parent = 0);

    class InstanceHelper;
    class MediaObjectPrivate;
};

}

#endif // MEDIAOBJECT_H
