#include "qytlib/videos/streams/streamsclient.h"
#include "qytlib/videos/streams/muxedstreaminfo.h"
#include "qytlib/videos/streams/videoonlystreaminfo.h"
#include "qytlib/videos/streams/audioonlystreaminfo.h"

#include "qytlib/utils/regexutils.h"
#include "qytlib/utils/urlutils.h"

#include "qytlib/reverseengineering/responses/istreaminfoprovider.h"
#include "qytlib/reverseengineering/responses/watchpage.h"
#include "qytlib/reverseengineering/heuristics.h"

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

    connect(streamContext, &StreamContext::ready, this, [this, videoId, streamManifest, streamContext, tries]() {
        processStreamContext(videoId, streamManifest, streamContext, tries);
    });

    return streamManifest;
}

/// Gets the HTTP Live Stream (HLS) manifest URL for the specified video (if it's a live video stream).
/// The LiveUrl emits LiveUrl::ready() when the url can be read. (Through LiveUrl->url())
LiveUrl* StreamsClient::getLiveStreamUrl(const Videos::VideoId &videoId)
{
    qDebug() << "StreamsClient::getHttpLiveStreamUrl(" << videoId << ")";

    auto *liveUrl = new LiveUrl(this);
    const auto *videoInfoResponse = Responses::VideoInfoResponse::get(m_networkManager, videoId, "", this);

    connect(videoInfoResponse, &Responses::VideoInfoResponse::ready, this, [ &videoId, liveUrl, videoInfoResponse ]() {
        auto *playerResponse = videoInfoResponse->playerResponse();

        if (!playerResponse)
        {
            qCWarning(ytStreamsClient()) << "Received PlayerResponse for livestream url, but it's null.";
            return;
        }

        if (!playerResponse->isVideoPlayable())
        {
            qCWarning(ytStreamsClient()) << "Error: Livestream is not playable" << playerResponse->videoPlayabilityError();
            return;
        }

        liveUrl->setUrl(playerResponse->hlsManifestUrl());

        if (liveUrl->url().isEmpty())
        {
            qCWarning(ytStreamsClient()) << "Error: Video" << videoId << "is not a livestream.";
        }

        liveUrl->setReady();
    });

    return liveUrl;
}

/// Try to load stream context from the embedded video page.
/// If video is not allowed to be embedded, the stream context does
/// not contain any streams.
StreamContext* StreamsClient::getStreamContextFromVideoInfo(const Videos::VideoId &videoId)
{
    qCDebug(ytStreamsClient()) << "StreamsClient::getStreamContextFromVideoInfo(" << videoId << ")";

    auto *streamContext = new StreamContext(this);

    // Step 1: Get EmbedPage
    const auto *embedPage = Responses::EmbedPage::get(m_networkManager, videoId, streamContext);

    connect(embedPage, &Responses::EmbedPage::ready, this, [ this, videoId, streamContext, embedPage ] () {
        const auto *playerConfig = embedPage->playerConfig();

        if (!playerConfig)
        {
            qCDebug(ytStreamsClient()) << "Received PlayerConfig  but it's a nullpointer" << videoId;
            return;
        }

        qCDebug(ytStreamsClient()) << "Received PlayerConfig" << videoId;

        // Step 2: Get PlayerSource
        const auto *playerSource = Responses::PlayerSource::get(m_networkManager, playerConfig->playerSourceUrl(), streamContext);

        connect(playerSource, &Responses::PlayerSource::ready, this, [ this, videoId, playerSource, streamContext ]() {
            qCDebug(ytStreamsClient()) << "Received PlayerSource" << videoId << "cipher operations:" << playerSource->ciphererOperations().length();

            // Step 3: Get VideoInfoResponse
            const auto *videoInfoResponse = Responses::VideoInfoResponse::get(m_networkManager, videoId, playerSource->sts(), streamContext);

            connect(videoInfoResponse, &Responses::VideoInfoResponse::ready, this, [ this, videoId, streamContext, playerSource, videoInfoResponse ]()
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
    const auto *watchPage = Responses::WatchPage::get(m_networkManager, videoId, streamContext);

    connect(watchPage, &Responses::WatchPage::ready, this, [ this, videoId, streamContext, watchPage ]() {
        const auto *playerConfig = watchPage->playerConfig();

        if (!playerConfig)
        {
            qCDebug(ytStreamsClient()) << "Received PlayerConfig but it's a nullpointer" << videoId;
            return;
        }

        qCDebug(ytStreamsClient()) << "Received PlayerConfig" << videoId;

        // Step 2: Get PlayerSource
        const auto *playerSource = Responses::PlayerSource::get(m_networkManager, playerConfig->playerSourceUrl(), streamContext);

        connect(playerSource, &Responses::PlayerSource::ready, this, [ this, videoId, streamContext, playerConfig, playerSource ]() {
            qCDebug(ytStreamsClient()) << "Received PlayerSource" << videoId;

            loadStreamContext(streamContext, playerConfig->playerResponse(), playerSource, playerConfig->streams());
        });
    });
}

/// Load all available streams and cipher operations into the StreamContext
void StreamsClient::loadStreamContext(StreamContext *streamContext, const Responses::PlayerResponse *playerResponse,
                                      const Responses::PlayerSource *playerSource, const QVector<Responses::IStreamInfoProvider*> &streams)
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
        const auto *dashManifest = Responses::DashManifest::get(m_networkManager, dashManifestUrl, playerSource->ciphererOperations(), streamContext);

        connect(dashManifest, &Responses::DashManifest::ready, this, [ streamInfoProviders, dashManifest, streamContext, playerSource ]() {
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
void StreamsClient::loadManifest(StreamManifest *streamManifest, const StreamContext *streamContext)
{
    qCDebug(ytStreamsClient()) << "StreamsClient::loadManifest()";

     // To make sure there are no duplicates streams, group them by tag
    QMap<int, IStreamInfo*> streams;
    const auto providers = streamContext->streamInfoProviders();

    for (const auto& streamInfo : providers)
    {
        const auto tag = streamInfo->tag();

        qCDebug(ytStreamsClient()) << "Tag" << tag;

        // NOTE: yt explode fetches content length from url if it's empty, but we will ignore that for now
        if (streamInfo->contentLength() <= 0)
        {
            qCDebug(ytStreamsClient()) << "Content length is <= 0, is stream url broken?";
        }

        // Some information, parents get later set to the stream info
        auto *container = new Container(streamInfo->container(), this);
        auto *fileSize = new FileSize(streamInfo->contentLength(), this);
        auto *bitrate = new Bitrate(streamInfo->bitrate());

        auto *stream = buildStreamInfo(streamInfo, streamContext, tag, container, fileSize, bitrate);

        if (!stream) continue;

        if (streams.contains(tag)) stream->deleteLater();

        streams.insert(tag, stream);
    }

    streamManifest->parse(streams.values());
}

void StreamsClient::processStreamContext(const Videos::VideoId &videoId, StreamManifest *streamManifest, StreamContext *streamContext, int *tries)
{
    qCDebug(ytStreamsClient()) << "StreamsClient::processStreamContext()";

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
            return;
        }

        // Did not find stream first try, start loading from watch page
        qCDebug(ytStreamsClient()) << "Did not work first try, trying a second time ...";
        (*tries)++;
        getStreamContextFromWatchPage(videoId, streamContext);
        return;
    }

    // Second try
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

QString StreamsClient::getStreamUrl(const Responses::IStreamInfoProvider *streamInfo, const StreamContext *streamContext)
{
    if (streamInfo->signature().isEmpty())
    {
        qCDebug(ytStreamsClient()) << "Signature is empty!";
        return streamInfo->url();
    }

    const auto signature = Cipher::Utils::decipher(streamContext->cipherOperations(), streamInfo->signature());
    return Utils::UrlUtils::setQueryParameter(streamInfo->url(), streamInfo->signatureParameter(), signature);
}

IStreamInfo *StreamsClient::buildStreamInfo(const Responses::IStreamInfoProvider *streamInfo, const StreamContext *streamContext, int tag, Container *container, FileSize *fileSize, Bitrate *bitrate)
{
    const auto url = StreamsClient::getStreamUrl(streamInfo, streamContext);

    // Muxed or Video-only
    if (!streamInfo->videoCodec().isEmpty())
    {
        const auto frameRate = streamInfo->framerate() > 0 ? streamInfo->framerate() : 24;
        const auto videoQualityLabel = !streamInfo->videoQualityLabel().isEmpty() ? streamInfo->videoQualityLabel() : Heuristics::videoQualityLabel(tag, frameRate) ;
        const auto videoQuality = Heuristics::videoQuality(videoQualityLabel);

        const auto videoWidth = streamInfo->videoWidth();
        const auto videoHeight = streamInfo->videoHeight();
        const auto videoResolution = videoWidth > 0 && videoHeight > 0 ? VideoResolution(videoWidth, videoHeight) : Heuristics::videoResolution(videoQuality);

        // Muxed
        if (!streamInfo->audioCodec().isEmpty())
        {
            return new MuxedStreamInfo(tag, url, container, fileSize, bitrate, streamInfo->audioCodec(),
                                       streamInfo->videoCodec(), videoQualityLabel, videoQuality,
                                       videoResolution, frameRate, this);
        }

        // Video-only
        return new VideoOnlyStreamInfo(tag, url, container, fileSize, bitrate, streamInfo->videoCodec(),
                                       videoQualityLabel, videoQuality, videoResolution, frameRate, this);
    }

    // Audio-only
    if (!streamInfo->audioCodec().isEmpty())
    {
        return new AudioOnlyStreamInfo(tag, url, container, fileSize, bitrate, streamInfo->audioCodec(), this);
    }

    qCDebug(ytStreamsClient()) << "Warning: Stream info doesn't contain audio/video codec information.";

    container->deleteLater();
    fileSize->deleteLater();
    bitrate->deleteLater();

    return nullptr;
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
