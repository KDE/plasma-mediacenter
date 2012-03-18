#include "flicklist.h"
#include "listwidget.h"

#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QPainter>
#include <QFontMetrics>
#include <QLabel>
#include <QSizeF>
#include <KLocale>

#include <plasma/svg.h>
#include <plasma/theme.h>
#include <Plasma/Label>

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
        QGraphicsGridLayout* toplay = new QGraphicsGridLayout;
        //toplay->setOrientation(Qt::Horizontal);

        Plasma::Label* speedLabel = new Plasma::Label(m_widget);
        speedLabel->setText(i18n("Scrolling speed:"));
        m_speed = new Plasma::LineEdit(m_widget);
        m_setSpeed = new Plasma::PushButton(m_widget);
        m_setSpeed->setText(i18n("Set Speed"));

        m_speed->setText(QString::number(m_listWidget->wiimote()->scrollSpeed()));
        connect(m_setSpeed, SIGNAL(clicked()), this, SLOT(setSpeed()));
        connect(m_speed, SIGNAL(returnPressed()), this, SLOT(setSpeed()));

        toplay->addItem(speedLabel, 0, 0);
        toplay->addItem(m_speed, 0, 1);
        toplay->addItem(m_setSpeed, 0, 2);

        Plasma::Label* velocityLabel = new Plasma::Label(m_widget);
        velocityLabel->setText(i18n("Flicking velocity:"));
        m_velocity = new Plasma::LineEdit(m_widget);
        m_setVelocity = new Plasma::PushButton(m_widget);
        m_setVelocity->setText(i18n("Set Velocity"));

        m_velocity->setText(QString::number(m_listWidget->velocity));
        connect(m_setVelocity, SIGNAL(clicked()), this, SLOT(setVelocity()));
        connect(m_velocity, SIGNAL(returnPressed()), this, SLOT(setVelocity()));

        toplay->addItem(velocityLabel, 1, 0);
        toplay->addItem(m_velocity, 1, 1);
        toplay->addItem(m_setVelocity, 1, 2);


        Plasma::Label* titleLabel = new Plasma::Label(m_widget);
        titleLabel->setText(i18n("<b>Wiimote Flicklist Demo</b>"));
        lay->addItem(titleLabel);

        Plasma::Label* infoLabel = new Plasma::Label(m_widget);
        infoLabel->setText(i18n("Point the wiimote at the ledbar, hold <b>B</b> pressed and move up and down to scroll."));
        infoLabel->nativeWidget()->setWordWrap(true);
        lay->addItem(infoLabel);

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

void FlickList::setVelocity()
{
    if (m_widget) {
        QString _s = m_velocity->text();
        m_listWidget->velocity = (qreal)(_s.toDouble());
        //m_listWidget->wiimote()->setScrollSpeed(s);
        kDebug() << "Set velocity to " << m_listWidget->velocity;
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
