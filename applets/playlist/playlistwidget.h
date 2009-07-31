/***************************************************************************
 *   Copyright 2009 by Alessandro Diaferia <alediaferia@gmail.com>         *
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
#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QGraphicsWidget>
#include <Plasma/DataEngine>
#include <QList>

namespace Plasma {
    class TreeView;
    class ComboBox;
}

class QStandardItemModel;
class QDBusInterface;
class QGraphicsSceneDragDropEvent;
class QEvent;
class QModelIndex;

class PlaylistUpdater;
class CoverUpdater;

class PlaylistWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    PlaylistWidget(QGraphicsItem *parent = 0);
    ~PlaylistWidget();

    bool multiplePlaylistsEnabled();

    QStringList medias();
    int length();

    bool eventFilter(QObject *, QEvent *);

public Q_SLOTS:
    void playlistUpdated(const QString &, const Plasma::DataEngine::Data &);
    void coverUpdated(const QString &, const Plasma::DataEngine::Data &);
    void setMultiplePlaylistsEnabled(bool);
    void append(const QList<QUrl> &);

protected Q_SLOTS:
    void slotPlaylistAdded(const QString &source);
    void slotCoverReady(const QString &source);
    void showPlaylist(const QString &playlistName);
    void updateColors();
    void removeFromPlaylist(const QModelIndex &index);
    void reloadCover(const QModelIndex &index);
    void slotMediaActivated(const QModelIndex &index);

Q_SIGNALS:
    void mediaActivated(const QString &media);

protected:
    void dropEvent(QGraphicsSceneDragDropEvent *event);
//    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
//    void mousePressEvent(QGraphicsSceneMouseEvents *event);
//    void mouseReleaseEvent(QGrpahicsSceneMouseEvent *event);

private:
    enum ConvenientRoles {
        CoverSourceRole = Qt::UserRole + 4,
        PathRole = Qt::UserRole + 5
    };

    Plasma::TreeView *m_treeView;
    Plasma::ComboBox *m_comboBox;
    Plasma::DataEngine *m_playlistEngine;
    Plasma::DataEngine *m_coverEngine;
    QStandardItemModel *m_model;

    QWidget *m_indicator;

    PlaylistUpdater *m_pupdater;
    CoverUpdater *m_cupdater;

    bool m_multiplePlaylists;
};


///// two useful helpers for source updates /////
class PlaylistUpdater : public QObject
{
    Q_OBJECT
public:
    PlaylistUpdater(PlaylistWidget *parent) : QObject(parent), widget(parent)
    {}

private:
    PlaylistWidget *widget;

public slots:
    void dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
    { widget->playlistUpdated(source, data); }
};

class CoverUpdater : public QObject
{
    Q_OBJECT
public:
    CoverUpdater(PlaylistWidget *parent) : QObject(parent), widget(parent)
    {}

private:
    PlaylistWidget *widget;

public slots:
    void dataUpdated(const QString &source, const Plasma::DataEngine::Data &data)
    { widget->coverUpdated(source, data); }
};

#endif
