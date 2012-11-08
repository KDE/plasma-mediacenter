/***************************************************************************
 *   Copyright 2012 by Deepak Mittal <dpac.mittal2@gmail.com>              *
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

class MEDIACENTER_EXPORT SubtitleProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY ( QString subtitle READ subtitle NOTIFY subtitleChanged );
    Q_PROPERTY ( QUrl filename READ filename WRITE setFilename );
    Q_PROPERTY ( qint64 subtitleTime READ subtitleTime WRITE setSubtitleTime NOTIFY subtitleTimeChanged );


public:
    QList<Subtitle> subs;
    QString currentSubtitle;
    qint64 currentSubtitleStartTime;
    qint64 currentSubtitleEndTime;
    qint64 currentVideoTime;
    QUrl subtitleFilename;

    void processFile();
    void subtitleAt(qint64 input);
    QString subtitle();
    qint64 subtitleTime();
    void setSubtitleTime(const qint64 &currtime);
    QUrl filename();
    void setFilename(const QUrl &name);

signals:
    void subtitleChanged();
    void subtitleTimeChanged();

};

#endif // SUBTITLEPROVIDER_H
