/***********************************************************************************
 *   Copyright 2014 Sinny Kumari <ksinny@gmail.com>          `                     *
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

#ifndef BALOOSEARCHMEDIASOURCE_H
#define BALOOSEARCHMEDIASOURCE_H

#include <mediacenter/abstractmediasource.h>

class SearchResultHandler;

class BalooSearchMediaSource : public MediaCenter::AbstractMediaSource
{
    Q_OBJECT
public:
    explicit BalooSearchMediaSource(QObject* parent = 0, const QVariantList& args = QVariantList());
    QHash< QString, QStringList > medialist;

protected:
    virtual void run();

private Q_SLOTS:
    void startQuerying();
    void handleNewFile(const QStringList &files);

private:
    QHash<QString, SearchResultHandler*> m_searchResultHandlers;
    QHash<QString, SearchResultHandler*> m_searchResultHandlersByMimeType;

    void queryForMediaType(const QString &type);
    QStringList m_allowedMimes;
};

#endif // BALOOSEARCHMEDIASOURCE_H
