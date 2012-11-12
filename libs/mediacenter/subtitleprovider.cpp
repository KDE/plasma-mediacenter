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

    QUrl f(m_subtitleFilename.toString().replace(QRegExp("(.*)\\.(.*)$"), "\\1.srt"));
    QFile file(f.toLocalFile());

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         qDebug() << "Unable to open subtitle file " << file.fileName();
        subs.clear();
        return;
    }

    QTextStream in(&file);
    QRegExp re("(\\d+)(?:\\s+)(\\d\\d):(\\d\\d):(\\d\\d),(\\d+)\\s*-->\\s*(\\d\\d):(\\d\\d):(\\d\\d),(\\d+)(?:\\s+)(.*)$");
    QString temp;

    while (!in.atEnd()) {
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

    //Binary search
    int start = 0;
    int end = subs.size() - 1;
    bool found = false;
    while(end >= start) {
      int mid = (start + end)/2;

      if( input < subs[mid].totalStartMillisec )
	  end = mid - 1;
      else if( input > subs[mid].totalEndMillisec )
	  start = mid + 1;
      else {
	  found = true;
	  m_currentSubtitle = subs[mid].text;
	  currentSubtitleStartTime = subs[mid].totalStartMillisec;
	  currentSubtitleEndTime = subs[mid].totalEndMillisec;
	  return;
      }
 
    }
    if(!found){
      m_currentSubtitle.clear();
    }
}


QString SubtitleProvider::subtitle()
{
    computeAndStoreSubtitle(m_currentVideoTime);
    return m_currentSubtitle;
}

qint64 SubtitleProvider::subtitleTime()
{
    return m_currentVideoTime;
}

void SubtitleProvider::setSubtitleTime(const qint64 &currtime)
{
    m_currentVideoTime = currtime;
    emit subtitleTimeChanged();
    emit subtitleChanged();
}

QUrl SubtitleProvider::filename()
{
    return m_subtitleFilename;
}

void SubtitleProvider::setFilename(const QUrl &name)
{
    m_subtitleFilename = name;
    processFile();
}

