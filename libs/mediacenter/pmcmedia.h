/***********************************************************************************
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>                                *
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

#ifndef PMCMEDIA_H
#define PMCMEDIA_H

#include <QString>
#include <QSharedPointer>

#include "mediacenter_export.h"

#include "media.h"

class MEDIACENTER_EXPORT PmcMedia : public QObject
{
    Q_OBJECT
public:
    explicit PmcMedia(const QString &url, QObject* parent = 0);

    void setMedia(const QSharedPointer< Media >& media);

    QString sha() const;
    QString title () const;
    QString url() const;
    QString thumbnail () const;
    QString type() const;
    QString album() const;
    QString artist() const;
    int duration() const;
    QDateTime createdAt() const;

Q_SIGNALS:
    void updated();

private:
    class Private;
    Private * const d;

    QString fileName() const;
};

#endif // PMCMEDIA_H
