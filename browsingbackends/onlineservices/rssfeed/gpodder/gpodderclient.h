#ifndef GPODDERCLIENT_H
#define GPODDERCLIENT_H

#include <QtCore/QObject>
#include <QNetworkReply>

#include <mygpo-qt/PodcastList.h>


class QNetworkAccessManager;

class GpodderClient : public QObject
{
	Q_OBJECT
public:
    GpodderClient ( QObject* parent = 0 );
    ~GpodderClient();
public:
	void topFeeds(const int count);
signals:
	void podcastList( const QList<QUrl> *list );
private slots:
	void networkError(QNetworkReply::NetworkError err);
	void requestFinished();
private:
	QNetworkAccessManager *m_nam;
	mygpo::PodcastListPtr *m_podcastlist;
};

#endif // GPODDERCLIENT_H
