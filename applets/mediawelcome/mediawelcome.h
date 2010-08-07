/***************************************************************************
 *   Copyright 2010 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#ifndef MEDIAWELCOME_H
#define MEDIAWELCOME_H

#include <mediacenter/homeapplet.h>

class QmlViewWrapper;
class BackendModel;

class MediaWelcome : public MediaCenter::HomeApplet
{
    Q_OBJECT
public:
    MediaWelcome(QObject *parent, const QVariantList &args = QVariantList());
    ~MediaWelcome();

    void init();

private slots:
    void itemActivated();

private:
    BackendModel *m_model;
    QmlViewWrapper *m_view;
};

#endif // MEDIAWELCOME_H
