/***************************************************************************
 *   Copyright 2009-2010 by Alessandro Diaferia <alediaferia@gmail.com>    *
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
#ifndef QMLVIEWWRAPPER_H
#define QMLVIEWWRAPPER_H

#include <QGraphicsWidget>

class QDeclarativeEngine;
class QDeclarativeComponent;
class QAbstractItemModel;

class QmlViewWrapper : public QGraphicsWidget
{
    Q_OBJECT
public:
    QmlViewWrapper(QGraphicsItem *parent = 0);
    ~QmlViewWrapper();

    /**
     * always call setModel before setting the qml file path
     */
    void setModel(QAbstractItemModel *model);
    void setQmlPath(const QString &path);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void resizeEvent(QGraphicsSceneResizeEvent *event);

signals:

public slots:
    void aquireRootComponent();
    void printError();

private:
    QDeclarativeEngine *m_engine;
    QDeclarativeComponent *m_mainComponent;
    QObject *m_rootObject;
    QAbstractItemModel *m_model;
    QObject *m_root;

};

#endif // QMLVIEWWRAPPER_H
