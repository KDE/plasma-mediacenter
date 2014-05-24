#include "seriesbackend.h"
#include "seriesmodel.h"

MEDIACENTER_EXPORT_BROWSINGBACKEND(SeriesBackend)

SeriesBackend::SeriesBackend(QObject* parent, const QVariantList& args): AbstractBrowsingBackend(parent, args) {

}

bool SeriesBackend::initImpl() {
    setModel(new SeriesModel());
    return true;
}

#include "seriesbackend.moc"