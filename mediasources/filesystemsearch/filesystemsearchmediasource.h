/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef FILESYSTEMSEARCHMEDIASOURCE_H
#define FILESYSTEMSEARCHMEDIASOURCE_H

#include <mediacenter/abstractmediasource.h>

#include <QMutex>

class QFileInfo;
class QDir;

/**
 * \brief Goes through the user's home dir and searches for media
 *
 * This MediaSource is supposed to be used for a better-than-nothing scenario
 * when none of Baloo or Nepomuk are installed. As such this MediaSource does
 * not attempt 100% correctness and is not configurable at all.
 */
class FilesystemSearchMediaSource : public MediaCenter::AbstractMediaSource
{
public:
    explicit FilesystemSearchMediaSource(QObject* parent = 0, const QVariantList&  = QVariantList());
    virtual ~FilesystemSearchMediaSource();

protected:
    virtual void run();

private:
    mutable QMutex m_quitMutex;
    bool m_quit;

    QStringList m_allowedMimes;

    bool shouldQuit() const;
    bool recursiveSearch(const QDir& dir);
    void stop();
    bool checkAndAddFile(const QFileInfo &fileInfo);
    void addFile(const QFileInfo &fileInfo, const QString &type);
};

#endif // FILESYSTEMSEARCHMEDIASOURCE_H
