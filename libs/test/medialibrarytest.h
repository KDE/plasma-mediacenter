/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef MEDIALIBRARYTEST_H
#define MEDIALIBRARYTEST_H

#include <QObject>
#include <QMetaType>
#include <QSharedPointer>
#include <mediacenter/mediavalidator.h>

class PmcArtist;
class PmcAlbum;
class PmcMedia;
class QSignalSpy;
class MediaLibrary;

class MediaLibraryTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void createsDbWhenNotPresent();

    void addsNewMediaAndItsMetadata();

    void shouldEmitUpdatedForMediaInsteadOfNewMediaWhenDataUpdated();

    void shouldNotEmitUpdatedWhenNothingUpdated();

    void shouldEmitUpdatedWhenAlbumOrArtistChanged();

    void shouldNotAddMediaForNonExistentFile();

    void shouldCleanupEntriesForNonExistentMedia();

private:
    QHash< int, QVariant > createTestMediaData() const;
    QHash< int, QVariant > createTestMediaDataWithAlbumArtist() const;
    QString pathToDatabase() const;
};

Q_DECLARE_METATYPE(QList<QSharedPointer<PmcMedia> >)
Q_DECLARE_METATYPE(QList<QSharedPointer<PmcAlbum> >)
Q_DECLARE_METATYPE(QList<QSharedPointer<PmcArtist> >)

#endif // MEDIALIBRARYTEST_H
