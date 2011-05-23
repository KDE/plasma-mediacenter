#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <Plasma/DataEngine>

class DataSource : public Plasma::DataEngine
{
    Q_OBJECT
    
    
public:
    DataSource(QObject* parent, const QVariantList& args);
    void init();
};

#endif

  