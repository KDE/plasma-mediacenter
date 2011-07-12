/*
    Copyright (C) 2010  Alessandro Diaferia <alediaferia@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#ifndef LOCALFILESABSTRACTBACKEND_H
#define LOCALFILESABSTRACTBACKEND_H

#include <mediacenter/abstractbrowsingbackend.h>
#include <Nepomuk/Query/Term>

#include "ui_configbase.h"

class KDirModel;

class LocalFilesAbstractBackend : public MediaCenter::AbstractBrowsingBackend
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl);
public:
    LocalFilesAbstractBackend(const QString &name, QObject* parent, const QVariantList& args);
    virtual ~LocalFilesAbstractBackend();

    virtual void createConfigurationInterface(KConfigDialog* parent);
    virtual void init();

    QString backendName() const;
    virtual void openUrl(const KUrl& url);
    void setUrl(const QString& url);
    QString url();
    Q_INVOKABLE  bool fileType(const QString& url);

private slots:
    void initModel();
    void configAccepted();
    void getWatchedDirsList();
    void removeItem();

private:
    void setBackendName(const QString &name);
    void setFolderNavigation();

protected:
    QString m_acceptedMimePrefix;
    Nepomuk::Query::Term m_term;

private:
    Ui::ConfigBase ui;
    QString m_backendName;
    QStringList m_mimeTypes;
    KUrl m_localUrl;
    QStringList m_watchedDirs;
    KDirModel *m_model;
    bool m_fromPlaces : 1;
    bool m_folderNavigation : 1;
    bool m_useNepomuk : 1;
};

#endif // LOCALFILESABSTRACTBACKEND_H
