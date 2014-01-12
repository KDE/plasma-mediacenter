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

#include "pmcmedia.h"

PmcMedia::PmcMedia(QSharedPointer<Media> media) :
                   m_media(media)
{

}

PmcMedia::~PmcMedia()
{

}

const QString& PmcMedia::sha() const
{
    return m_media->sha();
}

const QString& PmcMedia::thumbnail() const
{
    return m_media->thumbnail();
}

const QString& PmcMedia::title() const
{
    return m_media->title();
}

const QString& PmcMedia::type() const
{
    return m_media->type();
}

const QString& PmcMedia::url() const
{
    return m_media->url();
}

