/***************************************************************************
 *   Copyright 2014 Sujith Haridasan <sujith.haridasan@kdemail.net>        *
 *   Copyright 2014 Ashish Madeti <ashishmadeti@gmail.com>                 *
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

#ifndef INTERFACETOPLAYER_H
#define INTERFACETOPLAYER_H

#include "mediacenter/mediacenter_export.h"

#include <QDBusAbstractAdaptor>
#include <QDBusObjectPath>
#include <QPointer>

#include <Phonon/MediaSource>

class MEDIACENTER_EXPORT InterfaceToPlayer : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player") // Docs: http://www.mpris.org/2.1/spec/Player_Node.html

    Q_PROPERTY(QString PlaybackStatus READ PlaybackStatus)
    Q_PROPERTY(double rate READ Rate WRITE setRate NOTIFY rateChanged)
    Q_PROPERTY(double Rate READ Rate WRITE setRate)
    Q_PROPERTY(QVariantMap Metadata READ Metadata)
    Q_PROPERTY(double volume READ Volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(double Volume READ Volume WRITE setVolume)
    Q_PROPERTY(qlonglong position READ Position WRITE setPropertyPosition)
    Q_PROPERTY(qlonglong Position READ Position)
    Q_PROPERTY(double MinimumRate READ MinimumRate)
    Q_PROPERTY(double MaximumRate READ MaximumRate)
    Q_PROPERTY(bool CanGoNext READ CanGoNext)
    Q_PROPERTY(bool CanGoPrevious READ CanGoPrevious)
    Q_PROPERTY(bool CanPlay READ CanPlay)
    Q_PROPERTY(bool CanPause READ CanPause)
    Q_PROPERTY(bool CanControl READ CanControl)
    Q_PROPERTY(bool CanSeek READ CanSeek)
    Q_PROPERTY(bool paused READ paused WRITE setPaused)
    Q_PROPERTY(bool stopped READ stopped WRITE setStopped)
    Q_PROPERTY(QUrl currentTrack READ currentTrack WRITE setCurrentTrack)
    Q_PROPERTY(bool mediaPlayerPresent READ mediaPlayerPresent WRITE setMediaPlayerPresent)

    // Many properties need a duplicate starting with a small letter because
    // QML does not allow a property starting with a capital letter.

public:
    explicit InterfaceToPlayer(QObject* parent = 0);
    ~InterfaceToPlayer();

    QString PlaybackStatus() const;
    double Rate() const;
    QVariantMap Metadata() const;
    double Volume() const;
    qlonglong Position() const;
    double MinimumRate() const;
    double MaximumRate() const;
    bool CanGoNext() const;
    bool CanGoPrevious() const;
    bool CanPlay() const;
    bool CanPause() const;
    bool CanSeek() const;
    bool CanControl() const;
    bool stopped() const;
    bool paused() const;
    QUrl currentTrack() const;

protected:
    Q_INVOKABLE void emitSeeked(int pos);

signals:
    void Seeked(qlonglong Position) const;

    void rateChanged(double newRate) const;
    void volumeChanged(double newVol) const;
    void next() const;
    void previous() const;
    void pause() const;
    void playPause() const;
    void stop() const;
    void play() const;
    void seek(int offset) const;

public Q_SLOTS:
    void Next() const;
    void Previous() const;
    void Pause() const;
    void PlayPause();
    void Stop() const;
    void Play() const;
    void Seek(qlonglong Offset) const;
    void SetPosition(const QDBusObjectPath& trackId, qlonglong pos);
    void OpenUri(QString uri) const;

private:
    void signalPropertiesChange(const QVariantMap& properties) const;
    void loadMetadata();
    QString getTrackID();
    bool mediaPlayerPresent() const;
    void setMediaPlayerPresent(bool status);

    void setRate(double newRate);
    void setVolume(double volume);
    void setPropertyPosition(int newPositionInMs);
    void setCanPlay(bool newVal);
    void setCanPause(bool newVal);
    void setStopped(bool newVal);
    void setPaused(bool newVal);
    void setCurrentTrack(QUrl newTrack);

    QVariantMap m_metadata;
    QUrl m_currentTrack;
    double m_rate;
    double m_volume;
    bool m_paused;
    bool m_stopped;
    bool m_mediaPlayerPresent;
    qlonglong m_position;
};

#endif // INTERFACETOPLAYER_H
