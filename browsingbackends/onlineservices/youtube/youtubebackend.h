/***************************************************************************
 *   Copyright 2013 by Sinny Kumari <ksinny@gmail.com>                     *
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

#ifndef YOUTUBEBACKEND_H
#define YOUTUBEBACKEND_H

#include "youtubemodel.h"
#include "videodetailsmodel.h"

#include <libs/mediacenter/abstractbrowsingbackend.h>

/**
 * @class YoutubeBackend
 * @brief A backend that manages queries to Youtube.
*/
class YoutubeBackend : public MediaCenter::AbstractBrowsingBackend
{
    Q_OBJECT
public:
    YoutubeBackend(QObject *parent, const QVariantList &args);

    virtual QString backendCategory() const;
    bool expand (int row);
    virtual bool initImpl();
    virtual void search(const QString& searchTerm);
    virtual bool supportsSearch() const;

public Q_SLOTS:
    virtual bool goOneLevelUp();
    void realUrlFound();

private:
    YoutubeModel *m_youtubeModel;
    VideoDetailsModel *m_videoDetailsModel;
    bool m_expanded;  // to detrmine which model is currently set into backend
};

#endif
