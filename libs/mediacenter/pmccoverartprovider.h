#ifndef PMCCOVERARTPROVIDER_H
#define PMCCOVERARTPROVIDER_H

#include <QtDeclarative/QDeclarativeImageProvider>

#include "mediacenter_export.h"

class MEDIACENTER_EXPORT PmcCoverArtProvider : public QDeclarativeImageProvider
{
public:
    static const char *identificationString;
    PmcCoverArtProvider ();
    virtual QImage requestImage ( const QString& id, QSize* size, const QSize& requestedSize );
};

#endif // PMCCOVERARTPROVIDER_H
