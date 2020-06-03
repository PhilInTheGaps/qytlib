#include "qytlib/videos/closedcaptions/closedcaptionmanifest.h"
#include "qytlib/reverseengineering/responses/videoinforesponse.h"

Q_LOGGING_CATEGORY(ytClosedCaptionManifest, "yt.videos.cc.manifest");

using namespace YouTube::Videos::CC;

ClosedCaptionManifest *ClosedCaptionManifest::get(const VideoId &videoId, QNetworkAccessManager *networkManager, QObject *parent)
{
    qCDebug(ytClosedCaptionManifest()) << "ClosedCaptionManifest::get(" << videoId << ")";

    auto *manifest = new ClosedCaptionManifest(parent);
    auto *videoInfoResponse = Responses::VideoInfoResponse::get(networkManager, videoId, "", manifest);

    connect(videoInfoResponse, &Responses::VideoInfoResponse::ready, manifest, [=]() {
        qCDebug(ytClosedCaptionManifest()) << "ClosedCaptionManifest::get(): Received VideoInfoResponse";

        auto *playerResponse = videoInfoResponse->playerResponse();

         QList<ClosedCaptionTrackInfo*> trackInfos;

         for (const auto *track : playerResponse->closedCaptionTracks())
         {
             auto *info = new ClosedCaptionTrackInfo(
                         track->url(),
                         track->language(),
                         track->isAutoGenerated(),
                         parent);
             trackInfos.append(info);
         }

         videoInfoResponse->deleteLater();
         manifest->setTracks(trackInfos);
    });

    return manifest;
}

ClosedCaptionTrackInfo *ClosedCaptionManifest::getByLanguage(const QLocale &language) const
{
    for (auto *track : m_tracks)
    {
        if (track->language() == language) return track;
    }

    return nullptr;
}

void ClosedCaptionManifest::setTracks(QList<ClosedCaptionTrackInfo *> tracks)
{
    m_tracks = tracks;
    emit ready();
}
