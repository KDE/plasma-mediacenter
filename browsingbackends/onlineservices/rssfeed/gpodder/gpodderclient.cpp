#include "gpodderclient.h"

#include <mygpo-qt/ApiRequest.h>

#include <kdebug.h>

using namespace mygpo;

GpodderClient::GpodderClient ( QObject* parent ) :
	QObject ( parent ),
	m_nam(0),
	m_podcastlist(0)
{
	m_nam = new QNetworkAccessManager(this);
}

GpodderClient::~GpodderClient()
{
	m_nam->deleteLater();
}

void GpodderClient::topFeeds ( const int count )
{
	ApiRequest req(m_nam);
	m_podcastlist = new PodcastListPtr(req.toplist(count));
	connect(m_podcastlist->data(), SIGNAL(requestError(QNetworkReply::NetworkError)), this, SLOT(networkError(QNetworkReply::NetworkError)));
	connect(m_podcastlist->data(), SIGNAL(finished()), this, SLOT(requestFinished()));
}

void GpodderClient::networkError ( QNetworkReply::NetworkError err )
{
	//TODO
	kDebug() << "network err" << err;
}

void GpodderClient::requestFinished()
{
	QList<QUrl> urllist;
	QList<PodcastPtr> list;
	QList<PodcastPtr>::const_iterator iterator;

	list = m_podcastlist->data()->list();
	for (iterator = list.constBegin(); iterator != list.constEnd(); ++iterator) {
		urllist.append(iterator->data()->url());
	}

	emit podcastList(&urllist);
}
