#include "flicklist.h"
#include "listwidget.h"

#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <KLocale>

#include <plasma/svg.h>
#include <plasma/theme.h>

FlickList::FlickList(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
    m_widget(0)
{
    setBackgroundHints(DefaultBackground);
    setHasConfigurationInterface(false);
    (void*)graphicsWidget();
    setPopupIcon("infrared-remote");
    //resize(400, 400);
}


FlickList::~FlickList()
{
    if (hasFailedToLaunch()) {
        // Do some cleanup here
    } else {
        // Save settings
    }
}

QGraphicsWidget* FlickList::graphicsWidget()
{
    if (!m_widget) {
        m_widget = new QGraphicsWidget(this);
        m_listWidget = new ListWidget(m_widget);

        QGraphicsLinearLayout* lay = new QGraphicsLinearLayout(m_widget);
        lay->setOrientation(Qt::Vertical);
        QGraphicsLinearLayout* toplay = new QGraphicsLinearLayout;
        toplay->setOrientation(Qt::Horizontal);

        m_speed = new Plasma::LineEdit(m_widget);
        m_setSpeed = new Plasma::PushButton(m_widget);

        m_speed->setText(QString::number(m_listWidget->wiimote()->scrollSpeed()));
        connect(m_setSpeed, SIGNAL(clicked()), this, SLOT(setSpeed()));
        connect(m_speed, SIGNAL(returnPressed()), this, SLOT(setSpeed()));

        toplay->addItem(m_speed);
        toplay->addItem(m_setSpeed);
        lay->addItem(toplay);
        lay->addItem(m_listWidget);

    }
    return m_widget;
}

void FlickList::setSpeed()
{
    if (m_widget) {
        QString _s = m_speed->text();
        int s = _s.toInt();
        m_listWidget->wiimote()->setScrollSpeed(s);
        kDebug() << "Set speed to " << s;
    }
}

void FlickList::init()
{
    // A small demonstration of the setFailedToLaunch function
    if (0) {
        setFailedToLaunch(true, i18n("No world to say hello"));
    }
}

#include "flicklist.moc"
