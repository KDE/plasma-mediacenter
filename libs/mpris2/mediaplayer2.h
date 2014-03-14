/***************************************************************************
 *   Copyright 2014 Sujith Haridasan <sujith.haridasan@kdemail.net>        *
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

#include "mediacenter/mediacenter_export.h"

#include <QDBusAbstractAdaptor>
#include <QStringList>

class MEDIACENTER_EXPORT MediaPlayer2 : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")
    
    Q_PROPERTY(bool CanQuit READ CanQuit)
    Q_PROPERTY(bool CanSetFullscreen READ CanSetFullscreen)
    Q_PROPERTY(bool Fullscreen READ Fullscreen)
    
    public:
        explicit MediaPlayer2(QObject* parent = 0);
        ~MediaPlayer2();
        
        Q_INVOKABLE bool CanQuit() const;
        Q_INVOKABLE bool CanSetFullscreen() const;
        Q_INVOKABLE bool Fullscreen() const;
        QString Identity() const;

};