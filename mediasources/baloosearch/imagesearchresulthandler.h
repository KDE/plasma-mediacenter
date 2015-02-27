/***********************************************************************************
 *   Copyright 2014 Shantanu Tushar <shantanu@kde.org>                             *
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

#ifndef IMAGESEARCHRESULTHANDLER_H
#define IMAGESEARCHRESULTHANDLER_H

#include "searchresulthandler.h"

#include <QHash>

class ImageSearchResultHandler : public SearchResultHandler
{
    Q_OBJECT
public:
    ImageSearchResultHandler(MediaLibrary* mediaLibrary, QObject* parent);
    virtual QString supportedMediaType() const;

protected:
    virtual void handleResultImpl(const QString& filePath,
                                  const QHash< int, QVariant >& values);

private:
    unsigned int m_minimumImageSize;
    QHash<QString, QHash<int, QVariant>> m_initialValuesByUrl;
};

#endif // IMAGESEARCHRESULTHANDLER_H
