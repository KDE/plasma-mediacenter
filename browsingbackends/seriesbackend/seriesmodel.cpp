#include "seriesmodel.h"
#include <mediacenter/pmccoverartprovider.h>
#include <mediacenter/pmcmedia.h>
#include <mediacenter/singletonfactory.h>
#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

#include <QDebug>

SeriesModel::SeriesModel(QObject* parent): QAbstractListModel(parent)
{
    mediaData = SingletonFactory::instanceFor<MediaLibrary>()->getMedia("video");
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

    clusterer.buildCluster(mediaData);
    connect(&clusterer, SIGNAL(sizeChanged(int)), SLOT(clusterSizeUpdated(int)));
}

QVariant SeriesModel::data(const QModelIndex& index, int role) const
{
    switch (role) {
    case Qt::DisplayRole: {
        const QString display = currentCluster >= 0 ? clusterer.cluster(currentCluster)->mergedNodes().at(index.row())->label() : clusterer.label(index.row());
        return QString("(%1) %2").arg(currentCluster >= 0 ? 0 : clusterer.cluster(index.row())->mergedNodes().count()).arg(display);
    }
    case Qt::DecorationRole: {
        QString url = currentCluster >= 0 ? clusterer.cluster(currentCluster)->mergedNodes().at(index.row())->url() : clusterer.cluster(index.row())->url();
        return data(index, MediaCenter::IsExpandableRole).toBool() ? "folder-video" : PmcCoverArtProvider::qmlImageUriForMediaFileCover(url.prepend("file://"));
    }
    case MediaCenter::HideLabelRole:
        return false;
    case MediaCenter::IsExpandableRole:
        return currentCluster >= 0 ? false : clusterer.cluster(index.row())->mergedNodes().size() > 0;
    }
    return QVariant();
}

int SeriesModel::rowCount(const QModelIndex& parent) const
{
    return currentCluster >= 0 ? clusterer.cluster(currentCluster)->mergedNodes().size() : clusterer.clusterSize();
}

void SeriesModel::clusterSizeUpdated(int)
{
    reset();
}

bool SeriesModel::setCurrentCluster(int row)
{
    if (row == currentCluster) {
        return false;
    }
    beginResetModel();
    currentCluster = row;
    endResetModel();
    return true;
}


#include "seriesmodel.moc"
