/*
 *   Copyright 2015 Ashish Bansal <bansal.ashish096@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef FAKEMEDIASOURCE_H
#define FAKEMEDIASOURCE_H

#include <QObject>
#include <abstractmediasource.h>

class FakeMediaSource : public KMediaCollection::AbstractMediaSource

{
    Q_OBJECT
public:
    explicit FakeMediaSource(QObject* parent = 0, const QVariantList&  = QVariantList());
    inline QString getMediaFileName();

protected:
    void run();

private:
    void addMedia(QHash<int, QString> properties);

private Q_SLOTS:
    void parseJsonFile();
};

#endif // FAKEMEDIASOURCE_H
