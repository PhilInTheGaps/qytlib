#include "streamsclient.h"
#include "muxedstreaminfo.h"
#include "videoonlystreaminfo.h"
#include "audioonlystreaminfo.h"

#include "utils/regexutils.h"
#include "utils/urlutils.h"

#include "reverseengineering/responses/istreaminfoprovider.h"
#include "reverseengineering/responses/watchpage.h"
#include "reverseengineering/heuristics.h"

Q_LOGGING_CATEGORY(ytStreamsClient, "yt.videos.streams.client")

using namespace YouTube;
using namespace Videos::Streams;

/// Gets the manifest that contains information about available streams in the specified video.
StreamManifest* StreamsClient::getManifest(const Videos::VideoId &videoId)
{
    qCDebug(ytStreamsClient()) << "getManifest(videoId)" << videoId;

    auto *streamManifest = new StreamManifest(this);

    // We can try to extract the manifest from two sources: get_video_info and the video watch page.
    // In some cases one works, in some cases another does.

    // Start first option
    auto *streamContext = getStreamContextFromVideoInfo(videoId);
    auto *tries = new int(0);

    connect(streamContext, &StreamContext::ready, this, [ = ]() {
        loadManifest(streamManifest, streamContext);

        qCDebug(ytStreamsClient()) << "Loaded manifest, try:" << *tries;

        // First try
        if (*tries == 0)
        {
            // Found streams
            if (!streamManifest->streams().isEmpty())
            {
                qCDebug(ytStreamsClient()) << "Found streams first try.";
                streamManifest->setReady();
                streamContext->deleteLater();
                delete tries;
            }
            // Did not find stream first try, start loading from watch page
            else
            {
                qCDebug(ytStreamsClient()) << "Did not first try, trying a second time ...";
                (*tries)++;
                getStreamContextFromWatchPage(videoId, streamContext);
            }
        }
        // Second try
        else
        {
            // Still did not find any streams
            if (streamManifest->streams().isEmpty())
            {
                qCWarning(ytStreamsClient()) << "Error: Could not load stream manifest.";
            }
            else
            {
                qCDebug(ytStreamsClient()) << "Found streams the second try.";
            }

            streamManifest->setReady();
            streamContext->deleteLater();
            delete tries;
        }
    });

    return streamManifest;
}

/// Gets the HTTP Live Stream (HLS) manifest URL for the specified video (if it's a live video stream).
/// The LiveUrl emits LiveUrl::ready() when the url can be read. (Through LiveUrl->url())
LiveUrl* StreamsClient::getLiveStreamUrl(const Videos::VideoId &videoId)
{
    qDebug() << "StreamsClient::getHttpLiveStreamUrl(" << videoId << ")";

    auto *liveUrl = new LiveUrl(this);
    auto *videoInfoResponse = Responses::VideoInfoResponse::get(m_networkManager, videoId, "", this);

    connect(videoInfoResponse, &Responses::VideoInfoResponse::ready, this, [ = ]() {
        auto *playerResponse = videoInfoResponse->playerResponse();

        if (!playerResponse)
        {
            qCWarning(ytStreamsClient()) << "Received PlayerResponse for livestream url, but it's null.";
        }
        else if (!playerResponse->isVideoPlayable())
        {
            qCWarning(ytStreamsClient()) << "Error: Livestream is not playable" << playerResponse->videoPlayabilityError();
        }
        else
        {
            liveUrl->setUrl(playerResponse->hlsManifestUrl());

            if (liveUrl->url().isEmpty())
            {
                qCWarning(ytStreamsClient()) << "Error: Video" << videoId << "is not a livestream.";
            }

            liveUrl->setReady();
        }
    });

    return liveUrl;
}

/// *Not implemented* Gets the actual stream which is identified by the specified metadata.
void StreamsClient::get(IStreamInfo *streamInfo)
{
    // YouTube streams are often rate-limited -- they return data at about the same rate
    // as the actual video is going. This helps them avoid unnecessary bandwidth by not loading
    // all data eagerly. On the other hand, we want to download the streams as fast as possible,
    // so we'll be splitting the stream into small segments and retrieving them separately, to
    // work around rate limiting.

    // TODO: maybe implement segmented stream

}

/// Try to load stream context from the embedded video page.
/// If video is not allowed to be embedded, the stream context does
/// not contain any streams.
StreamContext* StreamsClient::getStreamContextFromVideoInfo(const Videos::VideoId &videoId)
{
    qCDebug(ytStreamsClient()) << "StreamsClient::getStreamContextFromVideoInfo(" << videoId << ")";

    auto *streamContext = new StreamContext(this);

    // Step 1: Get EmbedPage
    auto *embedPage = Responses::EmbedPage::get(m_networkManager, videoId, streamContext);

    connect(embedPage, &Responses::EmbedPage::ready, this, [ = ] () {
        auto *playerConfig = embedPage->playerConfig();

        if (!playerConfig)
        {
            qCDebug(ytStreamsClient()) << "Received PlayerConfig  but it's a nullpointer" << videoId;
            return;
        }

        qCDebug(ytStreamsClient()) << "Received PlayerConfig" << videoId;

        // Step 2: Get PlayerSource
        auto *playerSource = Responses::PlayerSource::get(m_networkManager, playerConfig->playerSourceUrl(), streamContext);

        connect(playerSource, &Responses::PlayerSource::ready, this, [ = ]() {
            qCDebug(ytStreamsClient()) << "Received PlayerSource" << videoId << "cipher operations:" << playerSource->ciphererOperations().length();

            // Step 3: Get VideoInfoResponse
            auto *videoInfoResponse = Responses::VideoInfoResponse::get(m_networkManager, videoId, playerSource->sts(), streamContext);

            connect(videoInfoResponse, &Responses::VideoInfoResponse::ready, this, [ = ]()
            {
                qCDebug(ytStreamsClient()) << "Received VideoInfoResponse" << videoId;

                auto playerResponse = videoInfoResponse->playerResponse();

                loadStreamContext(streamContext, playerResponse, playerSource, videoInfoResponse->streams());
            });
        });
    });

    return streamContext;
}

/// Try to load stream context from the watch page. Used as a fallback
/// if data can not be fetched from the embedded page.
void StreamsClient::getStreamContextFromWatchPage(const Videos::VideoId &videoId, StreamContext *streamContext)
{
    qCDebug(ytStreamsClient()) << "StreamsClient::getStreamContextFromWatchPage(" << videoId << ")";

    // Step 1: Get WatchPage
    auto *watchPage = Responses::WatchPage::get(m_networkManager, videoId, streamContext);

    connect(watchPage, &Responses::WatchPage::ready, this, [ = ]() {
        auto *playerConfig = watchPage->playerConfig();

        if (!playerConfig)
        {
         qCDebug(ytStreamsClient()) << "Received PlayerConfig but it's a nullpointer" << videoId;
         return;
        }

        qCDebug(ytStreamsClient()) << "Received PlayerConfig" << videoId;

        // Step 2: Get PlayerSource
        auto *playerSource = Responses::PlayerSource::get(m_networkManager, playerConfig->playerSourceUrl(), streamContext);

        connect(playerSource, &Responses::PlayerSource::ready, this, [ = ]() {
            qCDebug(ytStreamsClient()) << "Received PlayerSource" << videoId;

            loadStreamContext(streamContext, playerConfig->playerResponse(), playerSource, playerConfig->streams());
        });
    });
}

/// Load all available streams and cipher operations into the StreamContext
void StreamsClient::loadStreamContext(StreamContext *streamContext, Responses::PlayerResponse *playerResponse,
                                      Responses::PlayerSource *playerSource, QVector<Responses::IStreamInfoProvider*> streams)
{
    qCDebug(ytStreamsClient()) << "StreamsClient::loadStreamContext()";

    if (!playerResponse->previewVideoId().isEmpty())
    {
        qCWarning(ytStreamsClient()) << "Error: Video" << playerResponse->videoTitle() << "requires purchase, preview id:" << playerResponse->previewVideoId();
        streamContext->setReady();
        return;
    }

    if (!playerResponse->isVideoPlayable())
    {
        qCWarning(ytStreamsClient()) << "Error: Video" << playerResponse->videoTitle() << "is not playable:" << playerResponse->videoPlayabilityError();
        streamContext->setReady();
        return;
    }

    if (playerResponse->isLive())
    {
        qCWarning(ytStreamsClient()) << "Error: Video" << playerResponse->videoTitle() << "is actually a livestream";
        streamContext->setReady();
        return;
    }

    auto *streamInfoProviders = new QVector<Responses::IStreamInfoProvider*>;

    // Streams from video info or player config
    streamInfoProviders->append(streams);

    // Streams from player response
    streamInfoProviders->append(playerResponse->streams());

    // Streams from DASH manifest (only if dash manifest exists)
    auto dashManifestUrl = playerResponse->dashManifestUrl();

    if (!dashManifestUrl.isEmpty())
    {
        auto *dashManifest = Responses::DashManifest::get(m_networkManager, dashManifestUrl, playerSource->ciphererOperations(), streamContext);

        connect(dashManifest, &Responses::DashManifest::ready, this, [ = ]() {
            streamInfoProviders->append(dashManifest->streams());

            streamContext->setStreamInfoProviders(*streamInfoProviders);
            streamContext->setCipherOperations(playerSource->ciphererOperations());
            streamContext->setReady();
        });
    }
    else
    {
        streamContext->setStreamInfoProviders(*streamInfoProviders);
        streamContext->setCipherOperations(playerSource->ciphererOperations());
        streamContext->setReady();
    }
}

/// Load StreamManifest from StreamContext
void StreamsClient::loadManifest(StreamManifest *streamManifest, StreamContext *streamContext)
{
    qCDebug(ytStreamsClient()) << "StreamsClient::loadManifest()";

     // To make sure there are no duplicates streams, group them by tag
    QMap<int, IStreamInfo*> streams;
    auto providers = streamContext->streamInfoProviders();

    for (const auto& streamInfo : qAsConst(providers))
    {
        auto tag = streamInfo->tag();
        auto url = streamInfo->url();

        // Signature
        auto signature = streamInfo->signature();
        auto signatureParameter = streamInfo->signatureParameter();

        qCDebug(ytStreamsClient()) << "Tag" << tag;

        if (!signature.isEmpty())
        {
            signature = Cipher::Utils::decipher(streamContext->cipherOperations(), signature);

            url = Utils::UrlUtils::setQueryParameter(url, signatureParameter, signature);
        }
        else
        {
            qCDebug(ytStreamsClient()) << "Signature is empty!";
        }

        auto contentLength = streamInfo->contentLength();

        // NOTE: yt explode fetches content length from url if it's empty, but we will ignore that for now

        if (contentLength <= 0)
        {
            qCDebug(ytStreamsClient()) << "Content length is <= 0, is stream url broken?";
        }

        // Some information, parents get later set to the stream info
        auto *container = new Container(streamInfo->container(), this);
        auto *fileSize = new FileSize(contentLength, this);
        auto *bitrate = new Bitrate(streamInfo->bitrate());

        auto audioCodec = streamInfo->audioCodec();
        auto videoCodec = streamInfo->videoCodec();

        // Muxed or Video-only
        if (!videoCodec.isEmpty())
        {
            auto frameRate = streamInfo->framerate() > 0 ? streamInfo->framerate() : 24;
            auto videoQualityLabel = !streamInfo->videoQualityLabel().isEmpty() ? streamInfo->videoQualityLabel() : Heuristics::videoQualityLabel(tag, frameRate) ;
            auto videoQuality = Heuristics::videoQuality(videoQualityLabel);

            auto videoWidth = streamInfo->videoWidth();
            auto videoHeight = streamInfo->videoHeight();
            auto videoResolution = videoWidth > 0 && videoHeight > 0 ? VideoResolution(videoWidth, videoHeight) : Heuristics::videoResolution(videoQuality);

            // Muxed
            if (!audioCodec.isEmpty() && !streams.contains(tag))
            {
                streams.insert(tag, new MuxedStreamInfo(
                                   tag, url, container, fileSize,
                                   bitrate, audioCodec, videoCodec,
                                   videoQualityLabel, videoQuality,
                                   videoResolution, frameRate, this)
                               );
            }
            // Video-only
            else if (!streams.contains(tag))
            {
                streams.insert(tag, new VideoOnlyStreamInfo(
                                   tag, url, container, fileSize,
                                   bitrate, videoCodec,
                                   videoQualityLabel, videoQuality,
                                   videoResolution, frameRate, this)
                               );
            }
            else
            {
                container->deleteLater();
                fileSize->deleteLater();
                bitrate->deleteLater();
            }
        }
        // Audio-only
        else if (!audioCodec.isEmpty() && !streams.contains(tag))
        {
            streams.insert(tag, new AudioOnlyStreamInfo(
                               tag, url, container, fileSize,
                               bitrate, audioCodec, this)
                           );
        }
        else
        {
            qCDebug(ytStreamsClient()) << "Warning: Stream info doesn't contain audio/video codec information.";
            container->deleteLater();
            fileSize->deleteLater();
            bitrate->deleteLater();
        }
    }

    streamManifest->parse(streams.values());
}

void StreamContext::setStreamInfoProviders(const QVector<Responses::IStreamInfoProvider *> providers)
{
    m_streamInfoProviders = providers;

    for (const auto& provider : qAsConst(m_streamInfoProviders))
        provider->setParent(this);
}

void StreamContext::setCipherOperations(QVector<Cipher::ICipherOperation *> operations)
{
    m_cipherOperations = operations;

    for (const auto& operation : qAsConst(m_cipherOperations))
        operation->setParent(this);
}
