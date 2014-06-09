#ifndef SERIESBACKEND_H
#define SERIESBACKEND_H

#include <mediacenter/abstractbrowsingbackend.h>

class SeriesBackend : public MediaCenter::AbstractBrowsingBackend {
    Q_OBJECT
public:
    explicit SeriesBackend(QObject* parent, const QVariantList& args = QVariantList());
    virtual bool expand(int row);
protected:
    virtual bool initImpl();
};

#endif // SERIESBACKEND_H
