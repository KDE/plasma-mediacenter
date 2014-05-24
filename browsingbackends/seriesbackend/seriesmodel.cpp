#include "seriesmodel.h"
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
            return clusterer.label(index.row());
        case Qt::DecorationRole:
            return "inode-directory";
        case MediaCenter::HideLabelRole:
            return false;
        case MediaCenter::IsExpandableRole:
            return false;
    }
    return QVariant();
}

int SeriesModel::rowCount(const QModelIndex& parent) const {
    return clusterer.size();
}

void SeriesModel::clusterSizeUpdated(int) {
    reset();
}


#include "seriesmodel.moc"
