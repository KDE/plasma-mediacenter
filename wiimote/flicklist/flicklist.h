#ifndef FLICKLIST_HEADER
#define FLICKLIST_HEADER

#include <QGraphicsWidget>

#include <KIcon>

#include <Plasma/LineEdit>
#include <Plasma/PushButton>
#include <Plasma/PopupApplet>
#include <Plasma/Svg>

#include "listwidget.h"


// Define our plasma Applet
class FlickList : public Plasma::PopupApplet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        FlickList(QObject *parent, const QVariantList &args);
        ~FlickList();

        void init();

        QGraphicsWidget* graphicsWidget();

    private Q_SLOTS:
            void setSpeed();
            void setVelocity();

    private:
        QGraphicsWidget* m_widget;
        ListWidget *m_listWidget;
        Plasma::PushButton* m_setSpeed;
        Plasma::LineEdit* m_speed;
        Plasma::PushButton* m_setVelocity;
        Plasma::LineEdit* m_velocity;
};
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(flicklist, FlickList)

#endif
