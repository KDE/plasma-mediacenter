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

#include "modelmetadata.h"

class ModelMetadata::Private
{
public:
    Private()
    {
        model = 0;
        supportsSearch = false;
    }
    QString name;
    bool supportsSearch;
    QObject *model;
};

ModelMetadata::ModelMetadata(QObject* model, QObject* parent)
    : QObject(parent)
    , d(new Private())
{
    d->model = model;
}

QString ModelMetadata::name() const
{
    return d->name;
}

void ModelMetadata::setName(const QString& name)
{
    d->name = name;
    emit nameChanged();
}

void ModelMetadata::setSupportsSearch(bool supports)
{
    d->supportsSearch = supports;
}

bool ModelMetadata::supportsSearch() const
{
    return d->supportsSearch;
}

QObject* ModelMetadata::model()
{
    return d->model;
}

void ModelMetadata::setModel(QObject* model)
{
    if (d->model != model) {
        d->model = model;
        emit modelChanged();
    }
}
