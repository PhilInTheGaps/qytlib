#include "youtube.h"
#include "youtubeutils.h"

#include <QNetworkReply>
#include <QDebug>

YouTube::YouTube(QObject *parent) : QObject(parent)
{
    m_networkManager = new QNetworkAccessManager;
    m_networkManager->setRedirectPolicy(QNetworkRequest::RedirectPolicy::NoLessSafeRedirectPolicy);
    m_video = new YouTubeVideo(m_networkManager);

    connect(m_video, &YouTubeVideo::receivedVideo,                        this, &YouTube::receivedVideo);
    connect(m_video, &YouTubeVideo::receivedAuthorChannel,                this, &YouTube::receivedAuthorChannel);
    connect(m_video, &YouTubeVideo::receivedVideoMediaStreamInfos,        this, &YouTube::receivedVideoMediaStreamInfos);
    connect(m_video, &YouTubeVideo::receivedVideoClosedCaptionTrackInfos, this, &YouTube::receivedVideoClosedCaptionTrackInfos);
}

YouTube::~YouTube()
{
    m_video->deleteLater();
    m_networkManager->deleteLater();
}

int YouTube::getVideo(QString videoId)
{
    return m_video->getVideo(videoId);
}

int YouTube::getVideoAuthorChannel(QString videoId)
{
    return m_video->getVideoAuthorChannel(videoId);
}

int YouTube::getVideoMediaStreamInfos(QString videoId)
{
    return m_video->getVideoMediaStreamInfos(videoId);
}

int YouTube::getVideoAudioStreamInfo(QString videoId)
{
    return m_video->getVideoMediaStreamInfos(videoId, 2);
}

int YouTube::getVideoClosedCaptionTrackInfo(QString videoId)
{
    return m_video->getVideoClosedCaptionTrackInfo(videoId);
}

void YouTube::getPlaylist(QString playlistId, int maxPages)
{
}

void YouTube::getPlaylist(QString playlistId)
{
}

void YouTube::searchVideos(QString query, int maxPages)
{
}

void YouTube::searchVideos(QString query)
{
}

void YouTube::getChannelId(QString username)
{
}

void YouTube::getChannel(QString channelId)
{
}

void YouTube::getChannelUploads(QString channelId, int maxPages)
{
}

void YouTube::getMediaStream(MediaStreamInfo *info)
{
}

void YouTube::downloadMediaStream(MediaStreamInfo *info, QDataStream output, qreal *progress)
{
}
