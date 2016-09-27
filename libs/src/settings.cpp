/***************************************************************************
 *   Copyright 2013 by Shantanu Tushar <shantanu@kde.org>                  *
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

#include "settings.h"

#include <KSharedConfig>

Settings::Settings(QObject *parent) : QObject(parent)
{
    configGroup = KSharedConfig::openConfig("plasma-mediacenterrc")->group("General");
}

Settings::~Settings()
{
    sync();
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    configGroup.writeEntry(key, value);
}

QVariant Settings::value(const QString &key, const QVariant &defaultValue) const
{
    return configGroup.readEntry(key, defaultValue);
}

void Settings::sync()
{
    configGroup.sync();
}

bool Settings::contains(const QString &key) const
{
    return configGroup.hasKey(key);
}

void Settings::remove(const QString &key)
{
    configGroup.deleteEntry(key);
}
