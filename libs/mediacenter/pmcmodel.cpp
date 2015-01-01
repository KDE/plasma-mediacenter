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

#include "pmcmodel.h"

#include <QDebug>
#include <QAbstractItemModel>

class PmcModel::Private
{
public:
    Private()
    {
        model = 0;
        supportsSearch = false;
        isList = false;
        expanded = false;
    }
    QString name;
    bool supportsSearch;
    bool isList;
    QObject *model;
    QString headerText;
    bool expanded;
    QVariantMap parentData;
};

PmcModel::PmcModel(QObject* model, QObject* parent)
    : QObject(parent)
    , d(new Private())
{
    qRegisterMetaType<ParentData>("ParentData");
    d->model = model;
}

QString PmcModel::name() const
{
    return d->name;
}

void PmcModel::setName(const QString& name)
{
    d->name = name;
    emit nameChanged();
}

void PmcModel::setSupportsSearch(bool supports)
{
    d->supportsSearch = supports;
}

bool PmcModel::supportsSearch() const
{
    return d->supportsSearch;
}

void PmcModel::setIsList(bool isList)
{
    d->isList = isList;
}

bool PmcModel::isList() const
{
    return d->isList;
}

QObject* PmcModel::model() const
{
    return d->model;
}

void PmcModel::setModel(QObject* model)
{
    if (d->model != model) {
        d->model = model;
        emit modelChanged();
    }
}

QString PmcModel::headerText() const
{
    return d->headerText;
}

void PmcModel::setHeaderText(const QString& text)
{
    if (d->headerText != text) {
        d->headerText = text;
        emit headerTextChanged();
    }
}

bool PmcModel::expanded() const
{
    return d->expanded;
}

void PmcModel::setExpanded(bool expanded)
 {
     if (d->expanded != expanded) {
        d->expanded = expanded;
        emit expandedChanged();
    }
}

QVariantMap PmcModel::parentData() const
{
    qDebug() << "Parent Data asked" << d->parentData;
    return d->parentData;
}

void PmcModel::setParentData(const QMap<int, QVariant>& data)
{
    d->parentData["display"] = data.value(Qt::DisplayRole);
    d->parentData["decoration"] = data.value(Qt::DecorationRole);
    emit parentDataChanged();
}
