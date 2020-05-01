#include "videoclient.h"

#include <QUrl>
#include <QNetworkReply>

Q_LOGGING_CATEGORY(ytVideoClient, "yt.video.client")

using namespace YouTube;
using namespace YouTube::Videos;

VideoClient::VideoClient(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent), m_networkManager(networkManager)
{
    m_streamsClient = new Streams::StreamsClient(networkManager, this);
    m_closedCaptionsClient = new CC::ClosedCaptionClient(this);
}

Video* VideoClient::getVideo(const VideoId &videoId)
{
    qCDebug(ytVideoClient()) << "VideoClient::getVideo(" << videoId << ")";

    auto *video = new Video(videoId, this);

    // Step 1: Fetch VideoInfoResponse
    auto *videoInfoResponse = Responses::VideoInfoResponse::get(m_networkManager, videoId, "", this);

    connect(videoInfoResponse, &Responses::VideoInfoResponse::ready, this, [ = ]() {
        // Step 2: Fetch WatchPage
        auto *watchPage = Responses::WatchPage::get(m_networkManager, videoId, this);

        connect(watchPage, &Responses::WatchPage::ready, this, [ = ]() {
            auto playerResponse = videoInfoResponse->playerResponse();

            auto *engagement = new Common::Engagement(
                        playerResponse->videoViewCount(),
                        watchPage->videoLikeCount(),
                        watchPage->videoDislikeCount(),
                        video);

            video->setTitle(playerResponse->videoTitle());
            video->setAuthor(playerResponse->videoAuthor());
            video->setUploadDate(playerResponse->videoUploadDate());
            video->setDescription(playerResponse->description());
            video->setDuration(playerResponse->videoDuration());
            video->setKeywords(playerResponse->keywords());
            video->setEngagement(engagement);
            video->setIsReady(true);

            videoInfoResponse->deleteLater();
            watchPage->deleteLater();
        });
    });

    return video;
}
