#include "seriesbackend.h"
#include "seriesmodel.h"

MEDIACENTER_EXPORT_BROWSINGBACKEND(SeriesBackend)

SeriesBackend::SeriesBackend(QObject* parent, const QVariantList& args): AbstractBrowsingBackend(parent, args) {

}

bool SeriesBackend::initImpl() {
    setModel(new SeriesModel());
    return true;
}

bool SeriesBackend::expand(int row)
{
    static_cast<SeriesModel*>(model())->setCurrentCluster(row);
    return true;
}

#include "seriesbackend.moc"
