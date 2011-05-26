#ifndef MEDIACONTAINER_H
#define MEDIACONTAINER_H

#include <Plasma/DataContainer>

class MediaContainer : public Plasma::DataContainer
{
    Q_OBJECT

public:
    MediaContainer(QObject *parent = 0);

protected:
    void updateData();

};

#endif