/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#ifndef LOCALPICTURESPACKAGE_H
#define LOCALPICTURESPACKAGE_H

#include <mediabrowserlibs/modelpackage.h>

#include "ui_localpicturesconfig.h"

class QAbstractItemModel;
class KConfigDialog;
class KDirModel;

class LocalPicturesPackage : public ModelPackage
{
    Q_OBJECT
public:
    LocalPicturesPackage(QObject *parent, const QVariantList &args);
    ~LocalPicturesPackage();

    QAbstractItemModel *model();
    ModelPackage::BrowsingType browsingType() const;

    void createConfigurationInterface(KConfigDialog *parent);

    void init();

private slots:
    void configAccepted();

private:
    Ui::LocalPicturesConfig uiLocal;
    bool m_fromPlaces;
    bool m_folderNavigation;
    KUrl m_localUrl;
    QStringList m_mimeTypes;

    KDirModel *m_model;

private:
    void setFolderNavigation();
};

#endif // LOCALPICTURESPACKAGE_H
