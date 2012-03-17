#ifndef BACKENDSMODEL_H
#define BACKENDSMODEL_H

#include "mediacenter_export.h"

#include <QAbstractItemModel>

class QDeclarativeEngine;

namespace MediaCenter {
    class AbstractBrowsingBackend;
}

class MEDIACENTER_EXPORT BackendsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        ModelObjectRole = Qt::UserRole + 1
    };

    BackendsModel (QDeclarativeEngine *engine, QObject* parent = 0);
    virtual QVariant data (const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int rowCount (const QModelIndex& parent = QModelIndex()) const;

private:
    bool loadBrowsingBackend(MediaCenter::AbstractBrowsingBackend *backend, QDeclarativeEngine *engine);

    QList<MediaCenter::AbstractBrowsingBackend*> backends;
};

#endif // BACKENDSMODEL_H
