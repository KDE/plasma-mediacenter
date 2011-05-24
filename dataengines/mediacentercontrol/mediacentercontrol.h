#ifndef MEDIACENTERCONTROL_H
#define MEDIACENTERCONTROL_H

#include <Plasma/DataEngine>

class MediaCenterControl : public Plasma::DataEngine
{
    Q_OBJECT
    
    
public:
    MediaCenterControl(QObject* parent, const QVariantList& args);
    void init();
};

#endif

  