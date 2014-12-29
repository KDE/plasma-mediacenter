/***********************************************************************************
 *   Copyright 2013 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef MEDIACENTER_PMCMODEL_H
#define MEDIACENTER_PMCMODEL_H

#include <QObject>
#include <QMetaType>

#include "mediacenter_export.h"

class MEDIACENTER_EXPORT PmcModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(bool supportsSearch READ supportsSearch NOTIFY supportsSearchChanged)
    Q_PROPERTY(bool isList READ isList NOTIFY isListChanged)
    Q_PROPERTY(QObject* model READ model NOTIFY modelChanged)
    Q_PROPERTY(QString headerText READ headerText NOTIFY headerTextChanged)
    Q_PROPERTY(bool expanded READ expanded NOTIFY expandedChanged)
public:
    explicit PmcModel(QObject *model = 0, QObject* parent = 0);

    QString name() const;
    void setName(const QString &name);

    bool supportsSearch() const;
    void setSupportsSearch(bool supports);

    bool isList() const;
    void setIsList(bool isList);

    QObject *model() const;
    void setModel(QObject* model);

    QString headerText() const;
    void setHeaderText(const QString &text);

    bool expanded() const;
    void setExpanded(bool expanded);

Q_SIGNALS:
    void nameChanged();
    void supportsSearchChanged();
    void isListChanged();
    void modelChanged();
    void headerTextChanged();
    void expandedChanged();

private:
    class Private;
    Private * const d;
};

#endif // MEDIACENTER_PMCMODEL_H
