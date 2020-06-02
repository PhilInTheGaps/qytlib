#include "democlient.h"

DemoClient::DemoClient(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    videoClient = new YouTube::Videos::VideoClient(networkManager, this);
}

void DemoClient::loadVideo(const QString &url)
{
    auto *video = videoClient->getVideo(url);
    connect(video, &YouTube::Videos::Video::ready, this, &DemoClient::onVideoLoaded);
}

void DemoClient::onVideoLoaded()
{
    auto *video = qobject_cast<YouTube::Videos::Video*>(sender());

    if (!video) return;

    m_title = video->title();
    m_author = video->author();
    m_description = video->description();

    emit videoInfoChanged();
}
