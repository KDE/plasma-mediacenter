/*
  Copyright (C) 2014 by Sujith Haridasan <sujith.haridasan@kdemail.net>

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License or (at your option) version 3 or any later version
  accepted by the membership of KDE e.V. (or its successor appro-
  ved by the membership of KDE e.V.), which shall act as a proxy
  defined in Section 14 of version 3 of the license.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see http://www.gnu.org/licenses/.
*/


#ifndef APPLICATION_H
#define APPLICATION_H


#include <KUniqueApplication>


class MainWindow;


class Application : public KUniqueApplication
{
    Q_OBJECT

    public:
        explicit Application();
        virtual ~Application();

        virtual int newInstance();


    private:
        MainWindow* m_mainWindow;
};

#endif
