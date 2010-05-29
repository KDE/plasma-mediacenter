/*
    Copyright (C) 2010  Alessandro Diaferia <alediaferia@gmail.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include "sharedlayoutcomponentsmanager.h"
#include <kdemacros.h>
#include <KIcon>

#include <Plasma/IconWidget>

class SingletonPrivate
{
public:
    SharedLayoutComponentsManager instance;
};

K_GLOBAL_STATIC(SingletonPrivate, singletonObject)

SharedLayoutComponentsManager::SharedLayoutComponentsManager(QObject* parent): QObject(parent),
m_jumpToHome(new Plasma::IconWidget(KIcon("user-home"), QString())),
m_toggleControlBarAutohide(new Plasma::IconWidget(KIcon("mail-attachment"), QString())),
m_backgroundMusic(new Plasma::IconWidget(KIcon("folder-sound"), QString())),
m_backgroundVideo(new Plasma::IconWidget(KIcon("folder-video"), QString())),
m_backgroundPicture(new Plasma::IconWidget(KIcon("folder-image"), QString())),
m_backgroundPictureMode(false),
m_backgroundVideoMode(false),
m_backgroundMusicMode(false)
{
}

SharedLayoutComponentsManager::~SharedLayoutComponentsManager()
{
    delete m_backgroundMusic.data();
    delete m_backgroundPicture.data();
    delete m_backgroundVideo.data();
    delete m_jumpToHome.data();
    delete m_toggleControlBarAutohide.data();
}

SharedLayoutComponentsManager* SharedLayoutComponentsManager::self()
{
    return &singletonObject->instance;
}

Plasma::IconWidget* SharedLayoutComponentsManager::homeWidget() const
{
    return m_jumpToHome.data();
}

Plasma::IconWidget* SharedLayoutComponentsManager::barAutohideControlWidget() const
{
    return m_toggleControlBarAutohide.data();
}

Plasma::IconWidget* SharedLayoutComponentsManager::backgroundMusicWidget() const
{
    return m_backgroundMusic.data();
}

Plasma::IconWidget* SharedLayoutComponentsManager::backgroundPictureWidget() const
{
    return m_backgroundPicture.data();
}

Plasma::IconWidget* SharedLayoutComponentsManager::backgroundVideoWidget() const
{
    return m_backgroundVideo.data();
}

bool SharedLayoutComponentsManager::isBackgroundMusicMode() const
{
    return m_backgroundMusicMode;
}

bool SharedLayoutComponentsManager::isBackgroundPictureMode() const
{
    return m_backgroundPictureMode;
}

bool SharedLayoutComponentsManager::isBackgroundVideoMode() const
{
    return m_backgroundVideoMode;
}

void SharedLayoutComponentsManager::setBackgroundMusicMode(bool set)
{
    m_backgroundMusicMode = set;
}

void SharedLayoutComponentsManager::setBackgroundPictureMode(bool set)
{
    m_backgroundPictureMode = set;
}

void SharedLayoutComponentsManager::setBackgroundVideoMode(bool set)
{
    m_backgroundVideoMode = set;
}
