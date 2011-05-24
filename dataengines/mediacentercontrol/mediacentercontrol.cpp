#include "mediacentercontrol.h"
#include <Plasma/DataContainer>

MediaCenterControl::MediaCenterControl(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args)
{
     Q_UNUSED(args);
}

void MediaCenterControl::init()
{
    Plasma::DataContainer *source = new Plasma::DataContainer;
     source->setObjectName("MediaStatus");
     addSource(source);
     setData("MediaStatus","State","Playing");
     setData("MediaStatus","Progress",20); 
     setData("MediaStatus","MediaType", "Audio");
     setData("MediaStatus","Url","/home/Music/sintel.mp3");
     
}
    
K_EXPORT_PLASMA_DATAENGINE(org.kde.mediacentercontrol, MediaCenterControl)
#include "mediacentercontrol.moc"