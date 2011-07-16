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
#include "mediajob.h"
#include "kdebug.h"

MediaJob::MediaJob(Media* media,
        const QString &destination, const QString &operation, const QMap<QString, QVariant> &parameters,
        QObject *parent) : Plasma::ServiceJob(destination, operation, parameters, parent),
        m_media(media)
{

}

void MediaJob::start()
{
       if (!m_media) {
        setErrorText(i18n("'%1' cannot be found.", destination()));
        setError(-1);
        emitResult();
        return;
    }

    const QString operation(operationName());
    if (operation == "play") {
        if (m_media->canPlay()) {
            m_media->play();
        } else {
            setErrorText(i18n("'%1' cannot perform the action 'play'.", m_media->name()));
            setError(-1);
        }
    } else if (operation == "pause") {
        if (m_media->canPause()) {
            m_media->pause();
        } else {
            setErrorText(i18n("'%1' cannot perform the action 'pause'.", m_media->name()));
            setError(-1);
        }
    } else if (operation == "stop") {
        if (m_media->canStop()) {
            m_media->stop();
        } else {
            setErrorText(i18n("'%1' cannot perform the action 'stop'.", m_media->name()));
            setError(-1);
        }
    } else if (operation == "previous") {
        if (m_media->canGoPrevious()) {
            m_media->previous();
        } else {
            setErrorText(i18n("'%1' cannot perform the action 'previous'.", m_media->name()));
            setError(-1);
        }
    } else if (operation == "next") {
        if (m_media->canGoNext()) {
            m_media->next();
        } else {
            setErrorText(i18n("'%1' cannot perform the action 'next'.", m_media->name()));
            setError(-1);
        }
    } else if (operation == "volume") {
        if (m_media->canSetVolume()) {
            if (parameters().contains("level")) {
                qreal volume = parameters()["level"].toDouble();
                if (volume >= 0.0 && volume <= 1.0) {
                    m_media->setVolume(volume);
                } else {
                    setErrorText(i18n("The 'level' argument to the 'volume' command must be between 0 and 1."));
                    setError(-2);
                }
            } else {
                setErrorText(i18n("The 'volume' command requires a 'level' argument."));
                setError(-2);
            }
        } else {
            setErrorText(i18n("'%1' cannot perform the action 'volume'.", m_media->name()));
            setError(-1);
        }
    } else if (operation == "seek") {
        if (m_media->canSeek()) {
            if (parameters().contains("seconds")) {
                qreal time = parameters()["seconds"].toInt();
                kDebug() << time;
                if (time >= 0 && time <= m_media->length()) {
                    m_media->seek(time);
                   // bool dirtyBit = parameters()["dirty"].toBool();
                } else {
                    setErrorText(i18n("The 'seconds' argument to the 'seek' command must be "
                                      "between 0 and the length of the track."));
                    setError(-2);
                }
            } else {
                setErrorText(i18n("The 'seek' command requires a 'seconds' argument."));
                setError(-2);
            }
        } else {
            setErrorText(i18n("'%1' cannot perform the action 'seek'.", m_media->name()));
            setError(-1);
        }
    } else if (operation == "mediaProgress") {
        if (m_media->canMediaProgress()) {
            if (parameters().contains("seconds")) {
                qreal time = parameters()["seconds"].toInt();
                if (time >= 0 && time <= m_media->length()) {
                    m_media->mediaProgress(time);
                }
            }
            if (parameters().contains("mediaLength")) {
                qreal totalTime = parameters()["mediaLength"].toInt();
                if (totalTime >= 0) {
                    kDebug() << "updating legth of media progres";
                    m_media->setLength(totalTime);
                }
            }
        }
    } else if (operation == "url") {
        if (parameters().contains("mediaUrl")) {
            QString newUrl = parameters()["mediaUrl"].toString();
            kDebug() << "new Url to be set is  " << newUrl;
            m_media->setUrl(newUrl);
        }
    }
    emitResult();
} 

#include "mediajob.moc"
