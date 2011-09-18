/***************************************************************************
 *   Copyright 2011 by Sinny Kumari <ksinny@gmail.com>                     *
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

#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include <QtCore/QObject>

class Media : public QObject
{
    Q_OBJECT
public:
    enum State {
        Playing,
        Paused,
        Stopped
    };

    Media();
    QString name() const;

    int length() const;
    void setLength(int time);

    bool isRunning();
    State state();
    int position() const;
    qreal volume() const;

    bool canPlay() const;
    void play();

    bool canPause() const;
    void pause();

    bool canStop() const;
    void stop();

    bool canGoPrevious() const;
    void previous();

    bool canGoNext() const;
    void next();

    bool canSetVolume() const;
    void setVolume(qreal volume=0);

    bool canSeek() const;
    void seek(int time=0);

    bool canUpdateMediaProgress() const;
    void mediaProgress(int time=0);

    void setUrl(QString);
    QString getUrl() const;

    void setDirty(bool);
    bool getDirty() const;

    QString browsingState() const;
    void setBrowsingState(const QString &state);

    void setViewMode(bool);
    bool viewMode() const;

Q_SIGNALS:
    void mediaDataUpdated();

protected:
    void setName(const QString& name);

private:
    QString m_name;
    Media::State m_state;
    int m_position;
    qreal m_volume;
    int m_length;
    QString m_url;
    bool m_dirty;
    QString m_browsingState;
    bool m_viewMode;
};

#endif // MEDIA_H
