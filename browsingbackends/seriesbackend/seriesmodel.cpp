#include "seriesmodel.h"
#include <mediacenter/pmccoverartprovider.h>
#include <mediacenter/pmcmedia.h>
#include <mediacenter/singletonfactory.h>
#include <mediacenter/medialibrary.h>
#include <mediacenter/mediacenter.h>

SeriesModel::SeriesModel(QObject* parent): QAbstractListModel(parent)
{
    mediaData = SingletonFactory::instanceFor<MediaLibrary>()->getMedia("video");
    setRoleNames(MediaCenter::appendAdditionalMediaRoles(roleNames()));

    clusterer.buildCluster(mediaData);
    connect(&clusterer, SIGNAL(sizeChanged(int)), SLOT(clusterSizeUpdated(int)));
}

QVariant SeriesModel::data(const QModelIndex& index, int role) const {
    switch(role) {
        case Qt::DisplayRole:
            return currentCluster >= 0 ? clusterer.cluster(currentCluster)->children().at(index.row())->label() : clusterer.label(index.row());
        case Qt::DecorationRole: {
            QString url = currentCluster >= 0 ? clusterer.cluster(currentCluster)->children().at(index.row())->url() : clusterer.cluster(index.row())->url();
            return PmcCoverArtProvider::qmlImageUriForMediaFileCover(url.prepend("file://"));
        }
        case MediaCenter::HideLabelRole:
            return false;
        case MediaCenter::IsExpandableRole:
            return true;
    }
    return QVariant();
}

int SeriesModel::rowCount(const QModelIndex& parent) const {
    return currentCluster >= 0 ? clusterer.cluster(currentCluster)->children().size() : clusterer.size();
}

void SeriesModel::clusterSizeUpdated(int) {
    reset();
}

void SeriesModel::setCurrentCluster(int row)
{
    currentCluster = row;
    reset();
}


#include "seriesmodel.moc"
