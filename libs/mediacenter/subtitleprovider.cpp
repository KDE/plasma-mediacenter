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


#include "subtitleprovider.h"

void SubtitleProvider::processFile()
{
    currentSubtitleStartTime = currentSubtitleEndTime = 0;
    struct Subtitle t;

    QUrl f(subtitleFilename.toString().replace(QRegExp("(.*)\\.(.*)$"), "\\1.srt"));
    QFile file(f.toLocalFile());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open subtitle file " << file.fileName();
        return;
    }

    QTextStream in(&file);
    QRegExp re("(\\d+)(?:\\s+)(\\d\\d):(\\d\\d):(\\d\\d),(\\d+)\\s*-->\\s*(\\d\\d):(\\d\\d):(\\d\\d),(\\d+)(?:\\s+)(.*)$");

    while (!in.atEnd()) {
        QString temp;
        QString line = in.readLine();

        if(line.isEmpty()) {
            re.indexIn(temp);
            t.id = re.cap(1).toInt();
            t.startHr = re.cap(2).toInt();
            t.startMin = re.cap(3).toInt();
            t.startSec = re.cap(4).toInt();
            t.startMillisec = re.cap(5).toInt();
            t.totalStartMillisec = ( (t.startHr * 3600) + (t.startMin * 60) + (t.startSec) ) * 1000;
            t.endHr = re.cap(6).toInt();
            t.endMin = re.cap(7).toInt();
            t.endSec = re.cap(8).toInt();
            t.endMillisec = re.cap(9).toInt();
            t.totalEndMillisec = ( (t.endHr * 3600) + (t.endMin * 60) + (t.endSec) ) * 1000;
            t.text = re.cap(10);
            subs.append(t);
            temp.clear();
        }
        else {
            temp = temp + " " + line;
        }

    }
    file.close();
}

void SubtitleProvider::computeAndStoreSubtitle(qint64 input)
{
    struct Subtitle sub;
    struct Subtitle next;
    bool isLast = false;

    if( input >= currentSubtitleStartTime && input <= currentSubtitleEndTime )
        return;

    for(int i=0; i < subs.size(); ++i) {
        sub = subs[i];
        if( i < subs.size() -1 ) {
            next = subs[i+1];
        }
        else {
            isLast = true;
        }
        if(input > sub.totalStartMillisec && input < sub.totalEndMillisec) {
            currentSubtitle = sub.text;
            currentSubtitleStartTime = sub.totalStartMillisec;
            currentSubtitleEndTime = sub.totalEndMillisec;
            return;

        }
        else if( input > sub.totalEndMillisec && input < next.totalStartMillisec && !isLast) {
            currentSubtitle.clear();
            return;
        }
    }
    currentSubtitle.clear();
}


QString SubtitleProvider::subtitle()
{
    computeAndStoreSubtitle(currentVideoTime);
    return currentSubtitle;
}

qint64 SubtitleProvider::subtitleTime()
{
    return currentVideoTime;
}

void SubtitleProvider::setSubtitleTime(const qint64 &currtime)
{
    currentVideoTime = currtime;
    emit subtitleTimeChanged();
    emit subtitleChanged();
}

QUrl SubtitleProvider::filename()
{
    return subtitleFilename;
}

void SubtitleProvider::setFilename(const QUrl &name)
{
    subtitleFilename = name;
    processFile();
}

