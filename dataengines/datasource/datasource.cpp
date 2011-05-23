#include "datasource.h"
#include <Plasma/DataContainer>

DataSource::DataSource(QObject *parent, const QVariantList &args)
    : Plasma::DataEngine(parent, args)
{
     Q_UNUSED(args);
}

void DataSource::init()
{
    Plasma::DataContainer *source = new Plasma::DataContainer;
     source->setObjectName("MediaStatus");
     addSource(source);
     setData("MediaStatus","State","Playing");
     setData("MediaStatus","Progress",20); 
     setData("MediaStatus","MediaType", "Audio");
     setData("MediaStatus","Url","/home/Music/sintel.mp3");
     
}
    
K_EXPORT_PLASMA_DATAENGINE(singleDataSource, DataSource)
#include "datasource.moc"