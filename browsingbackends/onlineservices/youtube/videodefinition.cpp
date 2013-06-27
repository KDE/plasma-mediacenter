/***********************************************************************************
 *   Copyright Flavio Tordini <flavio.tordini@gmail.com>                           *
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

#include "videodefinition.h"

QStringList VideoDefinition::getDefinitionNames() {
    static QStringList definitionNames = QStringList() << "360p" << "720p" << "1080p";
    return definitionNames;
}

QList<int> VideoDefinition::getDefinitionCodes() {
    static QList<int> definitionCodes = QList<int>() << 18 << 22 << 37;
    return definitionCodes;
}

QHash<QString, int> VideoDefinition::getDefinitions() {
    static QHash<QString, int> definitions;
    if (definitions.isEmpty()) {
        definitions.insert("360p", 18);
        definitions.insert("720p", 22);
        definitions.insert("1080p", 37);
    }
    return definitions;
}

int VideoDefinition::getDefinitionCode(QString name) {
    return VideoDefinition::getDefinitions().value(name);
}

QString VideoDefinition::getDefinitionName(int code) {
    return getDefinitions().key(code);
}
