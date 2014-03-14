/***************************************************************************
 *   Copyright 2014 Sujith Haridasan <sujith.haridasan@kdemail.net>        *
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

#include "mediacenter/mediacenter_export.h"

#include <QDBusAbstractAdaptor>
#include <QDBusObjectPath>
#include <QPointer>

#include <Phonon/MediaSource>

class MEDIACENTER_EXPORT MediaPlayer2Player : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player") // Docs: http://www.mpris.org/2.1/spec/Player_Node.html

    Q_PROPERTY(bool Shuffle READ Shuffle WRITE setShuffle)
    Q_PROPERTY(double Volume READ Volume WRITE setVolume)
    Q_PROPERTY(qlonglong Position READ Position)
    Q_PROPERTY(bool CanGoNext READ CanGoNext)
    Q_PROPERTY(bool CanGoPrevious READ CanGoPrevious)
    Q_PROPERTY(bool CanPlay READ CanPlay)
    Q_PROPERTY(bool CanPause READ CanPause)
    Q_PROPERTY(bool CanSeek READ CanSeek)
    Q_PROPERTY(bool CanControl READ CanControl)

    public:
        explicit MediaPlayer2Player(QObject* parent = 0);
        ~MediaPlayer2Player();

        bool Shuffle() const;
        void setShuffle(bool shuffle) const;
        double Volume() const;
        Q_INVOKABLE void setVolume(double volume) const;
        qlonglong Position() const;
        bool CanGoNext() const;
        bool CanGoPrevious() const;
        bool CanPlay() const;
        bool CanPause() const;
        bool CanSeek() const;
        bool CanControl() const;

    signals:
        void Seeked(qlonglong Position) const;

    public slots:
        Q_INVOKABLE void Next() const;
        Q_INVOKABLE void Previous() const;
        Q_INVOKABLE void Pause() const;
        Q_INVOKABLE void PlayPause() const;
        Q_INVOKABLE void Stop() const;
        Q_INVOKABLE void Play() const;

    private slots:
        void volumeChanged(float newVol) const;
        void seeked(int newPos) const;

    private:
        void signalPropertiesChange(const QVariantMap& properties) const;

};
