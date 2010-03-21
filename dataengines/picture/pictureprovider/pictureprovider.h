/*
 *   Copyright 2009 Onur-Hayri Bakici <thehayro@gmail.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2 or
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

#ifndef PICTUREPROVIDER_H
#define PICTUREPROVIDER_H

#include <QObject>
#include <QList>
#include <QDate>
#include <QStringList>
#include <QVariantList>

#include "../picture_engine_export.h"

#include <KPluginFactory>
#include <KPluginLoader>

#define PICTUREPROVIDER_EXPORT( c ) \
    K_PLUGIN_FACTORY( MediaBrowserFactory, registerPlugin< c >(); ) \
    K_EXPORT_PLUGIN( MediaBrowserFactory("c") )


typedef struct {
    QString id;
    QString title;
    QString description;
    QStringList keywords;
    QString author;
    QDate updated;
    QDate published;
    QString link;
    uint width;
    uint height;
    uint size;
    QString albumId; // can be null, when picture has no album
    QString thumbnail;

} PicturePackage;

typedef struct {
    QString id;
    QString title;
    QList<PicturePackage*> photos;
} AlbumPackage;


class PICTURE_ENGINE_EXPORT PictureProvider : public QObject
{
    Q_OBJECT
public:
    PictureProvider(QObject *parent = 0, const QVariantList &args = QVariantList());
    virtual ~PictureProvider();

    /**
     * This method is used to actually perform the query to the
     * webservice. Additional arguments are used to filter the query.
     * Supported arguments will be specific for each plugin.
     */
    virtual void query(const QString &searchTerm, const QVariantList &args = QVariantList()) = 0;

    /**
     * This method returns a string that uniquely identifies
     * the picture provider service. Ids have to be lower-case.
     */
    QString serviceId();

protected:
    void setServiceId(const QString &id);

signals:
    /**
     * This signal should be emitted whenever results are ready to be
     * passed to the engine. @param query must contain the query string WITHOUT
     * the eventual query arguments. So searchTerm parameter from searchByQuery
     * method should be a valid @param query to pass to this signal.
     * @param pictures must be a list of correctly filled-in picturePackages.
     */
    void searchResult(const QString &query, const QList<PicturePackage> &pictures);

private:
    class PictureProviderPrivate;
    PictureProviderPrivate *d;
};

#endif // PICTUREPROVIDER_H
