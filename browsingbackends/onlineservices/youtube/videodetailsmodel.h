/***************************************************************************
 *   Copyright 2013 by Sinny Kumari <ksinny@gmail.com>                     *
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

#ifndef VIDEODETAILSMODEL_H
#define VIDEODETAILSMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QString>
#include <QUrl>

class VideoDetailsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    VideoDetailsModel(QObject *parent = 0);
    ~VideoDetailsModel();
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;
    void retriveRealUrl();
    void setVideoUrl(const QString &url);
    void setVideoThumbnail(const QString &thumbnail);

signals:
    void gotRealUrl();
public slots:
    void streamUrl(QUrl url);
private:
    QString m_videoThumbnail;
    QString m_videoUrl;
    QString m_videoTitle;
};

#endif  // VIDEODETAILSMODEL_H
