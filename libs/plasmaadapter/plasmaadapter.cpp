/***********************************************************************************
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>                                *
 *                                                                                 *
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

#include "../mediacenter/mediacenter_export.h"
#include <Plasma/Package>
#include <Plasma/Theme>

#include <QString>

extern "C"{
    void MEDIACENTER_EXPORT loadPlasmaTheme(){
        Plasma::Theme::defaultTheme()->setUseGlobalSettings(false);
        Plasma::Theme::defaultTheme()->setThemeName("oxygen");

    }

    QString MEDIACENTER_EXPORT getMainscriptPath() {
        Plasma::PackageStructure::Ptr structure = Plasma::PackageStructure::load("Plasma/Generic");
        Plasma::Package *package = new Plasma::Package(QString(), "org.kde.plasma.mediacenter", structure);

        return package->filePath("mainscript");
    }

    QString MEDIACENTER_EXPORT getImagesPath() {
        Plasma::PackageStructure::Ptr structure = Plasma::PackageStructure::load("Plasma/Generic");
        Plasma::Package *package = new Plasma::Package(QString(), "org.kde.plasma.mediacenter", structure);

        return package->filePath("images");
    }
}
