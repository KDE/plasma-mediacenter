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
    QString name;
    bool supports;
};

ModelMetadata::ModelMetadata(QObject* parent)
    : QObject(parent)
    , d(new Private())
{

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
    d->supports = supports;
}

bool ModelMetadata::supportsSearch() const
{
    return d->supports;
}
