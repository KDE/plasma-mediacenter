#include "seriesbackend.h"
#include "seriesmodel.h"
#include <QSortFilterProxyModel>

MEDIACENTER_EXPORT_BROWSINGBACKEND(SeriesBackend)

SeriesBackend::SeriesBackend(QObject* parent, const QVariantList& args): AbstractBrowsingBackend(parent, args) {

}

bool SeriesBackend::initImpl() {
    QSortFilterProxyModel *model = new QSortFilterProxyModel(this);
    model->setSourceModel(new SeriesModel());
    model->setFilterRole(MediaCenter::IsExpandableRole);
    model->setFilterFixedString("true");

    setModel(model);
    return true;
}

bool SeriesBackend::expand(int row)
{
    auto filterModel = static_cast<QSortFilterProxyModel*>(model());
    static_cast<SeriesModel*>(filterModel->sourceModel())->setCurrentCluster(row);
    filterModel->setFilterFixedString("false");
    return true;
}

bool SeriesBackend::goOneLevelUp()
{
    auto filterModel = static_cast<QSortFilterProxyModel*>(model());
    filterModel->setFilterFixedString("true");
    return static_cast<SeriesModel*>(filterModel->sourceModel())->setCurrentCluster(-1);
}

#include "seriesbackend.moc"
