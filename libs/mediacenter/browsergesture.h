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
#ifndef BROWSERGESTURE_H
#define BROWSERGESTURE_H

#include <QGesture>

#include "mediacenter_export.h"

namespace MediaCenter {
class MEDIACENTER_EXPORT BrowserGesture : public QGesture
{
    Q_OBJECT

    Q_PROPERTY(BrowserAction activeAction READ activeAction);

public:
    enum BrowserAction {
        BrowseLeftAction,
        BrowseRightAction,
        BrowseUpAction,
        BrowseDownAction,
        ActivateAction
    };

    explicit BrowserGesture(QObject *parent = 0);

    BrowserAction activeAction() const;

private:
    friend class GestureRecognizer;

    BrowserAction m_activeAction;

};
} // MediaCenter namespace

#endif // BROWSERGESTURE_H
