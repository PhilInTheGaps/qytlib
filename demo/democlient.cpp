#include "democlient.h"

using namespace YouTube::Videos;

DemoClient::DemoClient(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    videoClient = new VideoClient(networkManager, this);
}

void DemoClient::loadVideo(const QString &url)
{
    auto *video = videoClient->getVideo(url);
    connect(video, &Video::ready, this, &DemoClient::onVideoLoaded);

    auto *manifest = videoClient->streams()->getManifest(url);
    connect(manifest, &Streams::StreamManifest::ready, this, &DemoClient::onManifestLoaded);
}

void DemoClient::onVideoLoaded()
{
    auto *video = qobject_cast<Video*>(sender());
    if (!video) return;

    m_video = video;
    emit videoInfoChanged();
}

void DemoClient::onManifestLoaded()
{
    auto *manifest = qobject_cast<Streams::StreamManifest*>(sender());
    if (!manifest) return;

    auto *info = Streams::MuxedStreamInfo::withHighestBitrate(manifest->muxed());
    m_stream = info->url();

    emit streamChanged();
}
