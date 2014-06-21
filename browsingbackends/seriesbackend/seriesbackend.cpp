#include "seriesbackend.h"
#include "seriesmodel.h"
#include <mediacenter/modelmetadata.h>
#include <QSortFilterProxyModel>

MEDIACENTER_EXPORT_BROWSINGBACKEND(SeriesBackend)

SeriesBackend::SeriesBackend(QObject* parent, const QVariantList& args): AbstractBrowsingBackend(parent, args)
{

}

bool SeriesBackend::initImpl()
{
    QSortFilterProxyModel* model = new QSortFilterProxyModel(this);
    model->setSourceModel(new SeriesModel());
//     model->setFilterRole(MediaCenter::IsExpandableRole);
//     model->setFilterFixedString("true");

    ModelMetadata* metadata = new ModelMetadata(this);
    metadata->setModel(model);
    metadata->setSupportsSearch(true);

    setModel(metadata);
    return true;
}

bool SeriesBackend::expand(int row)
{
    auto filterModel = static_cast<QSortFilterProxyModel*>(model());

    QModelIndex filteredIndex = filterModel->index(row, 0);
    static_cast<SeriesModel*>(filterModel->sourceModel())->setCurrentCluster(filterModel->mapToSource(filteredIndex).row());
//     filterModel->setFilterFixedString("false");
    return true;
}

bool SeriesBackend::goOneLevelUp()
{
    auto filterModel = static_cast<QSortFilterProxyModel*>(model());
//     filterModel->setFilterFixedString("true");
    return static_cast<SeriesModel*>(filterModel->sourceModel())->setCurrentCluster(-1);
}

void SeriesBackend::search(const QString& searchTerm)
{
    auto filterModel = static_cast<QSortFilterProxyModel*>(model());
    filterModel->setFilterRole(Qt::DisplayRole);
    filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    filterModel->setFilterRegExp(QString(".*%1.*").arg(searchTerm));
}

#include "seriesbackend.moc"
