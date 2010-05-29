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

#ifndef SHAREDLAYOUTCOMPONENTSMANAGER_H
#define SHAREDLAYOUTCOMPONENTSMANAGER_H

#include <QtCore/QObject>
#include <QtCore/QWeakPointer>

namespace Plasma {
    class IconWidget;
}

class SingletonPrivate;
class SharedLayoutComponentsManager : public QObject
{

    friend class SingletonPrivate;
public:
    static SharedLayoutComponentsManager* self();

    Plasma::IconWidget* homeWidget() const;
    Plasma::IconWidget* barAutohideControlWidget() const;
    Plasma::IconWidget* backgroundMusicWidget() const;
    Plasma::IconWidget* backgroundVideoWidget() const;
    Plasma::IconWidget* backgroundPictureWidget() const;

    bool isBackgroundPictureMode() const;
    bool isBackgroundVideoMode() const;
    bool isBackgroundMusicMode() const;

    void setBackgroundPictureMode(bool);
    void setBackgroundVideoMode(bool);
    void setBackgroundMusicMode(bool);

private:
    SharedLayoutComponentsManager(QObject *parent = 0);
    ~SharedLayoutComponentsManager();

    QWeakPointer<Plasma::IconWidget> m_jumpToHome;
    QWeakPointer<Plasma::IconWidget> m_toggleControlBarAutohide;

    QWeakPointer<Plasma::IconWidget> m_backgroundMusic;
    QWeakPointer<Plasma::IconWidget> m_backgroundVideo;
    QWeakPointer<Plasma::IconWidget> m_backgroundPicture;

    bool m_backgroundPictureMode;
    bool m_backgroundVideoMode;
    bool m_backgroundMusicMode;
};

#endif // SHAREDLAYOUTCOMPONENTSMANAGER_H
