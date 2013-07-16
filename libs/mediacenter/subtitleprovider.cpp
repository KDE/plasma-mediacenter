/***********************************************************************************
 *   Copyright 2012 by Deepak Mittal <dpac.mittal2@gmail.com>                        
 *                                                                                 *
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


#include "subtitleprovider.h"

void SubtitleProvider::processFile_Srt()
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

        if (line.isEmpty()) {
            re.indexIn(temp);
            t.id = re.cap(1).toInt();
            t.startHr = re.cap(2).toInt();
            t.startMin = re.cap(3).toInt();
            t.startSec = re.cap(4).toInt();
            t.startMillisec = re.cap(5).toInt();
            t.totalStartMillisec = ((t.startHr * 3600) + (t.startMin * 60) + (t.startSec)) * 1000;
            t.endHr = re.cap(6).toInt();
            t.endMin = re.cap(7).toInt();
            t.endSec = re.cap(8).toInt();
            t.endMillisec = re.cap(9).toInt();
            t.totalEndMillisec = ((t.endHr * 3600) + (t.endMin * 60) + (t.endSec)) * 1000;
            t.text = re.cap(10);
            subs.append(t);
            temp.clear();
        } else {
            temp = temp + ' ' + line;
        }

    }
    file.close();
}

/* This function is to process a MicroDVD .sub subtitle file format */

void SubtitleProvider:: processFile_Sub()
{
    currentSubtitleStartTime = currentSubtitleEndTime = 0;
    struct Subtitle t;

    QUrl f(m_subtitleFilename.toString().replace(QRegExp("(.*)\\.(.*)$"), "\\1.sub"));
    QFile file(f.toLocalFile());
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//         qDebug() << "Unable to open subtitle file " << file.fileName();
        subs.clear();
        return;
    }

    QTextStream in(&file);
    
    QRegExp re("^\\{(\\d+)\}\\{(\\d+)\}(.*)$");   //Regular expression to match the contents of .sub format
    QString temp;

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.isEmpty()) {
            re.indexIn(temp);
	    
	    t.first=re.cap(1).toInt();
	    t.second=re.cap(2).toInt();
	    t.text = re.cap(3);          
	    
	    t.startSec = ( t.first/ (m_frameRate) ) ; 
	    t.startMillisec = (( t.first/ (m_frameRate) ) - (long) ( t.first/ (m_frameRate) ) ) * 1000 ;
	    t.startMin = ( t.first/ (m_frameRate) ) / 60 ;
	    t.startHr = 0;
	    
	    while(t.startMin >= 60 )
	    {
	      t.startHr = t.startHr + 1 ;
	      t.startMin = t.startMin - 60 ; 
	      
	    }
	    
	    t.totalStartMillisec = ( t.first/ (m_frameRate) ) * 1000 ;
	    
	    t.endMillisec = (( t.second/ (m_frameRate) ) - (long) ( t.second/ (m_frameRate) ) ) * 1000 ;
	    t.endSec = ( t.second/ (m_frameRate) ) ; 
	    t.endMin = ( t.second/ (m_frameRate) )/ 60 ;
	    t.endHr = 0;
	    
	    while(t.endMin >= 60 )
	    {
	      t.endHr = t.endHr + 1 ;
	      t.endMin = t.endMin - 60 ; 
	      
	    }
	    
	    t.totalEndMillisec = ( t.second/ (m_frameRate) ) ;
	 
            subs.append(t);
            temp.clear();
        } else {
            temp = temp + ' ' + line;
        }

    }
    file.close();
  
}
void SubtitleProvider::computeAndStoreSubtitle(qint64 input)
{
    struct Subtitle sub;
    struct Subtitle next;
    bool isLast = false;

    if (input >= currentSubtitleStartTime && input <= currentSubtitleEndTime)
        return;

    //Binary search
    int start = 0;
    int end = subs.size() - 1;
    bool found = false;
    while (end >= start) {
        int mid = (start + end) / 2;

        if (input < subs[mid].totalStartMillisec)
            end = mid - 1;
        else if (input > subs[mid].totalEndMillisec)
            start = mid + 1;
        else {
            found = true;
            m_currentSubtitle = subs[mid].text;
            currentSubtitleStartTime = subs[mid].totalStartMillisec;
            currentSubtitleEndTime = subs[mid].totalEndMillisec;
            return;
        }

    }
    if (!found) {
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

void SubtitleProvider::setSubtitleTime(const qint64& currtime)
{
    m_currentVideoTime = currtime;
    emit subtitleTimeChanged();
    emit subtitleChanged();
}

QUrl SubtitleProvider::filename()
{
    return m_subtitleFilename;
}

qreal SubtitleProvider::frameRate()
{
    return m_frameRate;
}

void SubtitleProvider::setFrameRate(qreal frame)
{
    m_frameRate=frame;
}


void SubtitleProvider::setFilename(const QUrl& name)
{
    m_subtitleFilename = name;
    
    QString url_string = m_subtitleFilename.toString();
    QFileInfo fi(url_string);
    QString extension= fi.suffix();
    
    if(extension == "srt") 
    {
      processFile_Srt();
    }
    
    else
    {
      processFile_Sub();
    }
    
}

