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
#include "modelpackage.h"

#include <KGlobal>
#include <KDebug>

class ModelPackage::ModelPackagePrivate
{
public:
    ModelPackagePrivate(ModelPackage *q) : q(q),
    cfInterface(false)
    {}

    ModelPackage *q;
    bool cfInterface;
};

ModelPackage::ModelPackage(QObject *parent, const QVariantList &args) : QObject(parent),
d(new ModelPackagePrivate(this))
{
    Q_UNUSED(args);
}

ModelPackage::~ModelPackage()
{
}

bool ModelPackage::hasConfigurationInterface()
{
    return d->cfInterface;
}

void ModelPackage::setHasConfigurationInterface(bool hasInterface)
{
    d->cfInterface = hasInterface;
}

void ModelPackage::createConfigurationInterface(KConfigDialog *parent)
{
    Q_UNUSED(parent);
}

KConfigGroup ModelPackage::config()
{
    kDebug() << KGlobal::config()->name();
    return KConfigGroup(KGlobal::config(), "ModelPackages");
}

void ModelPackage::init()
{}

void ModelPackage::setAllowedMediaTypes(const MediaCenter::MediaTypes &type)
{
    m_allowedMediaTypes = type;
}

MediaCenter::MediaTypes ModelPackage::allowedMediaTypes()
{
    return m_allowedMediaTypes;
}

