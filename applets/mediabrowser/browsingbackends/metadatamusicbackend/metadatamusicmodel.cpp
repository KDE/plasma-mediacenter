/***************************************************************************
 *   Copyright 2011 Sinny Kumari <ksinny@gmail.com>                        *
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


#include "metadatamusicmodel.h"

#include <Plasma/DataEngineManager>

#include <KDebug>

#include <QtGui/QIcon>

class MetadataMusicModel::Private
{
public:
    Private() {
        engine = 0;
        engineManager = 0;
    }

    Plasma::DataEngine* engine;
    Plasma::DataEngineManager* engineManager;
    QList<QObject*> musicList;
};

MetadataMusicModel::MetadataMusicModel (QObject* parent)
    : QAbstractItemModel (parent)
    , d (new Private())
{
    QHash<int, QByteArray> roles = roleNames();
    roles[MediaUrlRole] = "mediaUrl";
    setRoleNames(roles);

    d->engineManager = Plasma::DataEngineManager::self();
    d->engine = d->engineManager->loadEngine ("org.kde.active.metadata");
    d->engine->connectSource ("ResourcesOfType:Audio", this);
}

MetadataMusicModel::~MetadataMusicModel()
{
    delete d;
}

void MetadataMusicModel::dataUpdated (const QString& sourceName, const Plasma::DataEngine::Data& data)
{
    Plasma::DataEngine::DataIterator it (data);

    while (it.hasNext()) {
        it.next();

        QObject* musicObject = new QObject (this);
        musicObject->setObjectName (it.key());

        Plasma::DataEngine::Data data = it.value().value<Plasma::DataEngine::Data>();
        musicObject->setProperty ("mediaUrl", data.value ("url"));
        musicObject->setProperty ("icon", data.value("icon"));
        musicObject->setProperty("fileName", data.value("fileName"));

        d->musicList.append (musicObject);
    }
}

QVariant MetadataMusicModel::data (const QModelIndex& index, int role) const
{
    if (index.row() >= d->musicList.size()) {
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        return d->musicList.at (index.row())->property("fileName");
        break;
    case Qt::DecorationRole:
        return d->musicList.at (index.row())->property("icon");
        break;
    case MediaUrlRole:
        return d->musicList.at (index.row())->property("mediaUrl");
        break;
    }

    return QVariant();
}

int MetadataMusicModel::columnCount (const QModelIndex& parent) const
{
    return 1;
}

int MetadataMusicModel::rowCount (const QModelIndex& parent) const
{
    return d->musicList.size();
}

QModelIndex MetadataMusicModel::parent (const QModelIndex& child) const
{
    return QModelIndex();
}

QModelIndex MetadataMusicModel::index (int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return createIndex (row, column);
}

#include "metadatamusicmodel.moc"
