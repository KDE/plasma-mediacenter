/***************************************************************************
 *   Copyright 2011 by Sinny Kumari <ksinny@gmail.com>                     *
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
#ifndef MEDIASERVICE_H
#define MEDIASERVICE_H

#include <Plasma/Service>
#include "media.h"

class MediaService : public Plasma::Service
{
    Q_OBJECT
    
public:
    MediaService(Media* media, QObject* parent);
    
public slots:
    void enableMediaOperations();
 
protected:
    Plasma::ServiceJob* createJob(const QString &operation, QMap<QString, QVariant> &parameters);
    
private:
    Media* m_media;
};

#endif // MEDIASERVICE_H
