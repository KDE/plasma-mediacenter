/***********************************************************************************
 *   Copyright 2012 by Deepak Mittal <dpac.mittal2@gmail.com>                      *
 *   Copyright 2013 by Akshay Ratan <akshayratan@gmail.com>                                                                              *
 *                                                                                 *
 *   This library is free software; you can redistribute it and/or                 *
 *   modify it under the terms of the GNU Lesser General Public                    *
 *   License as published by the Free Software Foundation; either                  *
 *   version 2.1 of the License, or (at your option) any later version.            *
 *                                                                                 *
 *   This library is distributed in the hope that it will be useful,               *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of                *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU             *
 *   Lesser General Public License for more details.                               *
 *                                                                                 *
 *   You should have received a copy of the GNU Lesser General Public              *
 *   License along with this library.  If not, see <http://www.gnu.org/licenses/>. *
 ***********************************************************************************/


#ifndef SUBTITLEPROVIDER_H
#define SUBTITLEPROVIDER_H

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QUrl>

#include "mediacenter_export.h"

struct Subtitle {
    int id;
    int startHr;
    int startMin;
    int startSec;
    int startMillisec;
    qint64 totalStartMillisec;
    int endHr;
    int endMin;
    int endSec;
    int endMillisec;
    qint64 totalEndMillisec;
    QString text;
};

/**
  * @class SubtitleProvider @author Deepak Mittal
  * @brief Subtitle Provider for video files
  *
  * @description  This class is a basis to provide the srt subtitles to a video file.
  *
  */
class MEDIACENTER_EXPORT SubtitleProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString subtitle READ subtitle NOTIFY subtitleChanged);
    Q_PROPERTY(QUrl filename READ filename WRITE setFilename);
    Q_PROPERTY(qint64 subtitleTime READ subtitleTime WRITE setSubtitleTime NOTIFY subtitleTimeChanged);

public:
    QString subtitle();
    qint64 subtitleTime();
    void setSubtitleTime(const qint64& currtime);
    QUrl filename();
    void setFilename(const QUrl& name);

signals:
    void subtitleChanged();
    void subtitleTimeChanged();

private:
    QList<Subtitle> subs;
    QString m_currentSubtitle;
    qint64 currentSubtitleStartTime;
    qint64 currentSubtitleEndTime;
    qint64 m_currentVideoTime;
    QUrl m_subtitleFilename;

    void processFile();
    void computeAndStoreSubtitle(qint64 input);
};

#endif // SUBTITLEPROVIDER_H
