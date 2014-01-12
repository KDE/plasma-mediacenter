/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef MEDIA_H
#define MEDIA_H

#include <QString>
#include <QObject>

#include <odb/core.hxx>

#pragma db object
class Media : public QObject
{
    Q_OBJECT
public:
    Media(const QString &type, const QString& title, const QString& url,
           const QString& thumbnail, QObject* parent = 0);

    const QString& sha() const;

    const QString& title () const;
    void setTitle(const QString &title);

    const QString& url() const;
    void setUrl(const QString &url);

    const QString& thumbnail () const;
    void setThumbnail(const QString &thumbnail);

    const QString& type() const;
    void setType(const QString &type);

    static QString calculateSha(const QString& url);

signals:
    void updated();

private:
    Media();
    friend class odb::access;

    #pragma db id
    QString m_sha;
    QString m_title;
    QString m_url;
    QString m_thumbnail;
    QString m_type;

    class Private;
    #pragma db transient
    Private * const d;

    inline void emitUpdate();
};

#endif // MEDIA_H
