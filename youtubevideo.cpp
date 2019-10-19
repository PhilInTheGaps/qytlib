#include "youtubevideo.h"
#include "youtubeutils.h"

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDateTime>
#include <QDebug>
#include <memory>

#include "exceptions/videounavailableexception.h"
#include "exceptions/videorequirespurchaseexception.h"
#include "exceptions/unrecognizedstructureexception.h"

#include "cipheroperations/slicecipheroperation.h"
#include "cipheroperations/swapcipheroperation.h"
#include "cipheroperations/reversecipheroperation.h"
#include "cipheroperations/extensions.h"
#include "internal/heuristics.h"

YouTubeVideo::YouTubeVideo(QNetworkAccessManager *networkManager)
    : m_networkManager(networkManager)
{
    connect(this, &YouTubeVideo::receivedPlayerConfiguration, this, &YouTubeVideo::onReceivedPlayerConfiguration);
    connect(this, &YouTubeVideo::receivedCipherOperations,    this, &YouTubeVideo::onReceivedCipherOperations);
}

int YouTubeVideo::getVideoInfoMap(QString videoId)
{
    // This parameter does magic and a lot of videos don't work without it
    auto eurl = QUrl::toPercentEncoding(QString("https://youtube.googleapis.com/v/%1").arg(videoId));

    // Execute request
    auto url       = QUrl(QString("https://youtube.com/get_video_info?video_id=%1&el=embedded&eurl=%2&hl=en").arg(videoId).arg(QString(eurl)));
    auto reply     = m_networkManager->get(QNetworkRequest(url));
    auto requestId = m_requestCount++;

    connect(reply, &QNetworkReply::finished, [ = ]() {
        auto raw = reply->readAll();

        QUrlQuery query(raw);
        emit receivedVideoInfoMap(query, requestId);

        reply->deleteLater();
    });

    return requestId;
}

int YouTubeVideo::getVideoWatchPageHtml(QString videoId)
{
    auto url       = QUrl(QString("https://youtube.com/watch?v=%1&disable_polymer=true&bpctr=9999999999&hl=en").arg(videoId));
    auto reply     = m_networkManager->get(QNetworkRequest(url));
    auto requestId = m_requestCount++;

    connect(reply, &QNetworkReply::finished, [ = ]() {
        auto raw  = reply->readAll();
        auto html = QGumboDocument::parse(raw);
        emit receivedVideoWatchPageHtml(html.rootNode(), requestId);

        reply->deleteLater();
    });

    return requestId;
}

int YouTubeVideo::getVideoEmbedPageHtml(QString videoId)
{
    auto url       = QUrl(QString("https://youtube.com/embed/%1?disable_polymer=true&hl=en").arg(videoId));
    auto reply     = m_networkManager->get(QNetworkRequest(url));
    auto requestId = m_requestCount++;

    connect(reply, &QNetworkReply::finished, [ = ]() {
        auto raw  = reply->readAll();
        auto html = QGumboDocument::parse(raw);
        emit receivedVideoEmbedPageHtml(html.rootNode(), requestId);

        reply->deleteLater();
    });

    return requestId;
}

int YouTubeVideo::getDashManifestXml(QUrl url)
{
    auto reply     = m_networkManager->get(QNetworkRequest(url));
    auto requestId = m_requestCount++;

    connect(reply, &QNetworkReply::finished, [ = ]() {
        auto raw = reply->readAll();
        emit receivedDashManifestXml(raw, requestId);

        reply->deleteLater();
    });

    return requestId;
}

int YouTubeVideo::getVideo(QString videoId)
{
    if (videoId.isEmpty()) return -1;

    auto requestId = m_requestCount++;

    if (!YouTubeUtils::validateVideoId(videoId))
    {
        throw std::invalid_argument(QString("Invalid YouTube video ID [%1].").arg(videoId).toUtf8());
    }

    // Get video info dictionary
    std::unique_ptr<QMetaObject::Connection> pconn{ new QMetaObject::Connection };
    QMetaObject::Connection& conn = *pconn;
    auto tempRequestId1           = getVideoInfoMap(videoId);
    conn = connect(this, &YouTubeVideo::receivedVideoInfoMap, [ = ](QUrlQuery infoMap, int reqId) {
        if (reqId != tempRequestId1) return;

        disconnect(conn);

        // Get player response JSON
        auto playerResponseJson = QJsonDocument::fromJson(QUrl::fromPercentEncoding(infoMap.queryItemValue("player_response").toUtf8()).toUtf8());

        // If video is unavailable - throw
        if (playerResponseJson.object().value("playabilityStatus").toObject().value("status") == "error")
        {
            throw new VideoUnavailableException(videoId, QString("Video [%1] is unavailable.").arg(videoId));
        }

        // Extract video info
        auto videoDetails     = playerResponseJson.object().value("videoDetails").toObject();
        auto videoAuthor      = videoDetails.value("author").toString();
        auto videoTitle       = videoDetails.value("title").toString().replace("+", " ");
        auto videoDescription = videoDetails.value("shortDescription").toString();
        auto videoViewCount   = videoDetails.value("viewCount").toInt();
        qint64 videoDuration  = videoDetails.value("lengthSeconds").toString().toInt();

        QStringList videoKeywords;

        for (auto v : videoDetails.value("keywords").toArray()) videoKeywords.append(v.toString());

        // Get video watch page HTML
        std::unique_ptr<QMetaObject::Connection>pconn2{ new QMetaObject::Connection };
        QMetaObject::Connection& conn2 = *pconn2;
        auto tempRequestId2            = getVideoWatchPageHtml(videoId);
        conn2                          = connect(this, &YouTubeVideo::receivedVideoWatchPageHtml, [ = ](QGumboNode videoWatchPageHtml, int reqId) {
            if (reqId != tempRequestId2) return;

            disconnect(conn2);

            // Extract upload date
            auto videoUploadDate = QDateTime(); // TODO

            /*var videoUploadDate =
               videoWatchPageHtml.GetElementsBySelector("meta[itemprop=\"datePublished\"]")
                .First().GetAttribute("content").Value.ParseDateTimeOffset("yyyy-MM-dd");*/

            // Extract like count
            auto videoLikeCountRaw = videoWatchPageHtml.getElementsByClassName("like-button-renderer-like-button").front().innerText();
            auto videoLikeCount    = videoLikeCountRaw.toInt();

            // Extract dislike count
            auto videoDislikeCountRaw = videoWatchPageHtml.getElementsByClassName("like-button-renderer-dislike-button").front().innerText();
            auto videoDislikeCount    = videoDislikeCountRaw.toInt();

            // Create statistics and thumbnails
            auto statistics = Statistics(videoViewCount, videoLikeCount, videoDislikeCount);
            auto thumbnails = ThumbnailSet(videoId);

            emit receivedVideo(Video(videoId, videoAuthor, videoUploadDate, videoTitle, videoDescription, thumbnails, videoDuration, videoKeywords, statistics), requestId);
        });
    });

    return requestId;
}

int YouTubeVideo::getPlayerConfiguration(QString videoId, int requestId, int requestMode)
{
    // Get video embed page HTML
    std::unique_ptr<QMetaObject::Connection> pconn{ new QMetaObject::Connection };
    QMetaObject::Connection& conn = *pconn;
    auto tempRequestId1           = getVideoEmbedPageHtml(videoId);

    conn = connect(this, &YouTubeVideo::receivedVideoEmbedPageHtml, [ = ](QGumboNode videoEmbedPageHtml, int reqId)
    {
        if (reqId != tempRequestId1) return;

        disconnect(conn);

        // Get player config JSON
        auto playerConfigRaw = videoEmbedPageHtml.getElementsByTagName(HtmlTag::SCRIPT);
        QJsonDocument playerConfigJson;

        for (auto c : playerConfigRaw)
        {
            QRegularExpression regex("yt\\.setConfig\\({'PLAYER_CONFIG':(.*)}\\);");
            auto match = regex.match(c.innerText());

            if (match.hasMatch() && !match.captured(1).isEmpty())
            {
                playerConfigJson = QJsonDocument::fromJson(match.captured(1).toUtf8());
                break;
            }
        }

        // Extract player source URL
        auto playerSourceUrl = QUrl("https://youtube.com" + playerConfigJson.object().value("assets").toObject().value("js").toString());

        // Get video info dictionary
        auto requestedAt = QDateTime::currentDateTime();

        std::unique_ptr<QMetaObject::Connection>pconn2{ new QMetaObject::Connection };
        QMetaObject::Connection& conn2 = *pconn2;
        auto tempRequestId2            = getVideoInfoMap(videoId);

        conn2 = connect(this, &YouTubeVideo::receivedVideoInfoMap, [ = ](QUrlQuery videoInfoMap, int reqId)
        {
            if (reqId != tempRequestId2) return;

            disconnect(conn2);

            // Get player response JSON
            QJsonDocument playerResponseJson;

            playerResponseJson = QJsonDocument::fromJson(QUrl::fromPercentEncoding(videoInfoMap.queryItemValue("player_response").toUtf8()).toUtf8());

            auto playabilityStatus = playerResponseJson.object().value("playbilityStatus").toObject();

            // If video is unavailable - throw
            if (playabilityStatus.value("status").toString() == "error")
            {
                throw new VideoUnavailableException(videoId, QString("Video [%1] is unavailable.").arg(videoId));
            }

            // If there is no error - extract info and return
            auto errorReason = playabilityStatus.value("reason").toString();

            if (errorReason.isEmpty())
            {
                auto videoDetails  = playerResponseJson.object().value("videoDetails").toObject();
                auto streamingData = playerResponseJson.object().value("streamingData").toObject();

                // Extract whether the video is a live stream
                auto isLiveStream = videoDetails.value("isLive").toBool();

                // Extract valid until date
                auto validUntil = requestedAt.addSecs(streamingData.value("expiresInSeconds").toInt());

                // Extract stream info
                auto hlsManifestUrl                = isLiveStream ? streamingData.value("hlsManifestUrl").toString() : "";
                auto dashManifestUrl               = !isLiveStream ? streamingData.value("dashManifestUrl").toString() : "";
                auto muxedStreamInfosUrlEncoded    = !isLiveStream ? videoInfoMap.queryItemValue("url_encoded_fmt_stream_map") : "";
                auto adaptiveStreamInfosUrlEncoded = !isLiveStream ? videoInfoMap.queryItemValue("adaptive_fmts") : "";

                emit receivedPlayerConfiguration(PlayerConfiguration(playerSourceUrl, dashManifestUrl, hlsManifestUrl,
                                                                     muxedStreamInfosUrlEncoded, adaptiveStreamInfosUrlEncoded, validUntil), requestId, requestMode);
            }

            // If the video requires purchase - throw (approach one)

            auto previewVideoId = playabilityStatus.value("errorScreen").toObject().value("playerLegacyDesktopYpcTrailerRenderer").toObject().value("trailerVideoId").toString();

            if (!previewVideoId.isEmpty())
            {
                throw new VideoRequiresPurchaseException(videoId, previewVideoId, QString("Video [%1] is unplayable because it requires purchase.").arg(videoId));
            }


            // If the video requires purchase - throw (approach two)
            auto previewVideoInfoRaw =
                playabilityStatus.value("playabilityStatus").toObject().value("errorScreen").toObject().value("ypcTrailerRenderer").toObject().value("playerVars").toString();

            if (!previewVideoInfoRaw.isEmpty())
            {
                auto previewVideoInfoMap = QUrlQuery(previewVideoInfoRaw);
                auto previewVideoId      = previewVideoInfoMap.queryItemValue("video_id");

                throw new VideoRequiresPurchaseException(videoId, previewVideoId, QString("Video [%1] is unplayable because it requires purchase.").arg(videoId));
            }
        });
    });

    return requestId;

    // TODO: Get from watch page
}

int YouTubeVideo::getCipherOperations(QUrl playerSourceUrl, int requestId, int mode, QString args)
{
    // If already in cache - return
    if (m_cipherOperationsCache.contains(playerSourceUrl.toString()))
    {
        emit receivedCipherOperations(m_cipherOperationsCache[playerSourceUrl.toString()], args, requestId, mode);
        return requestId;
    }

    // Get player source
    auto reply = m_networkManager->get(QNetworkRequest(playerSourceUrl));

    connect(reply, &QNetworkReply::finished, [ = ]()
    {
        auto raw = reply->readAll();
        reply->deleteLater();

        // Find the name of the function that handles deciphering
        auto decipherFuncName = YouTubeUtils::regexMatch(raw, "(\\w+)=function\\(\\w+\\){(\\w+)=\\2\\.split\\(\\x22{2}\\);.*?return\\s+\\2\\.join\\(\\x22{2}\\)}");

        if (decipherFuncName.isEmpty())
        {
            throw new UnrecognizedStructureException(
                "Could not find signature decipherer function name. Please report this issue on GitHub.");
        }

        // Find the body of the function
        auto decipherFuncBody = YouTubeUtils::regexMatch(raw, "(?!h\\.)" + QRegularExpression::escape(decipherFuncName) + "=function\\(\\w+\\)\\{(.*?)\\}");

        if (decipherFuncBody.isEmpty())
        {
            throw new UnrecognizedStructureException(
                "Could not find signature decipherer function body. Please report this issue on GitHub.");
        }

        // Split the function body into statements
        auto deciphererFuncBodyStatements = decipherFuncBody.split(';');

        // Find the name of block that defines functions used in decipherer
        auto deciphererDefinitionName = YouTubeUtils::regexMatch(decipherFuncBody, "(\\w+).\\w+\\(\\w+,\\d+\\);");

        // Find the body of the function
        auto deciphererDefinitionBody = YouTubeUtils::regexMatch(
            raw,
            "var\\s+" +
            QRegularExpression::escape(deciphererDefinitionName) +
            "=\\{(\\w+:function\\(\\w+(,\\w+)?\\)\\{(.*?)\\}),?\\};",
            QRegularExpression::DotMatchesEverythingOption, 0);

        // Identify cipher functions
        QList<CipherOperation *>operations;

        // Analyze statements to determine cipher function names
        for (auto statement : deciphererFuncBodyStatements)
        {
            // Get the name of the function called in this statement
            auto calledFuncName = YouTubeUtils::regexMatch(statement, "\\w+(?:.|\\[)(\\\"?\\w+(?:\\\"\")?)\\]?\\(");

            if (calledFuncName.isEmpty()) continue;

            // Slice
            if (YouTubeUtils::regexHasMatch(deciphererDefinitionBody, QRegularExpression::escape(calledFuncName) + ":\\bfunction\\b\\([a],b\\).(\\breturn\\b)?.?\\w+\\."))
            {
                auto index = YouTubeUtils::regexMatch(statement, "\\(\\w+,(\\d+)\\)").toInt();
                operations.append(new SliceCipherOperation(index));
            }

            // Swap
            if (YouTubeUtils::regexHasMatch(deciphererDefinitionBody, QRegularExpression::escape(calledFuncName) + ":\\bfunction\\b\\(\\w+\\,\\w\\).\\bvar\\b.\\bc=a\\b"))
            {
                auto index = YouTubeUtils::regexMatch(statement, "\\(\\w+,(\\d+)\\)").toInt();
                operations.append(new SwapCipherOperation(index));
            }

            // Reverse
            if (YouTubeUtils::regexHasMatch(deciphererDefinitionBody, QRegularExpression::escape(calledFuncName) + ":\\bfunction\\b\\(\\w+\\)"))
            {
                operations.append(new ReverseCipherOperation());
            }
        }

        m_cipherOperationsCache.insert(playerSourceUrl.toString(), operations);

        emit receivedCipherOperations(operations, args, requestId, mode);
    });

    return requestId;
}

// TODO
int YouTubeVideo::getVideoAuthorChannel(QString videoId)
{
    return 0;
}

int YouTubeVideo::getVideoMediaStreamInfos(QString videoId, int requestMode)
{
    if (videoId.isEmpty()) return -1;

    auto requestId = m_requestCount;

    if (!YouTubeUtils::validateVideoId(videoId))
    {
        throw std::invalid_argument(QString("Invalid YouTube video ID [%1].").arg(videoId).toUtf8());
    }

    // Get player configuration
    getPlayerConfiguration(videoId, requestId, requestMode);

    return requestId;
}

void YouTubeVideo::onReceivedPlayerConfiguration(PlayerConfiguration playerConfiguration, int requestId, int requestMode)
{
    auto muxedStreamInfoMaps    = QUrl::fromPercentEncoding(playerConfiguration.muxedStreamInfosUrlEncoded().toUtf8()).split(',');
    auto adaptiveStreamInfoMaps = QUrl::fromPercentEncoding(playerConfiguration.adaptiveStreamInfosUrlEncoded().toUtf8()).split(',');
    int  count;

    switch (requestMode)
    {
    case 0:
        count = muxedStreamInfoMaps.count() + adaptiveStreamInfoMaps.count();
        break;

    case 1:
        count = muxedStreamInfoMaps.count();
        break;

    case 2:
    case 3:
        count = adaptiveStreamInfoMaps.count();
        break;
    }

    // Get muxed stream infos
    if ((requestMode == 0) || (requestMode == 1))
    {
        connect(this, &YouTubeVideo::receivedMuxedStreamInfo, [ = ](MuxedStreamInfo *info, int reqId)
        {
            m_mediaStreamInfoCache[requestId].append(info);

            if (m_mediaStreamInfoCache[requestId].count() == count) onReceivedAllStreamInfos(playerConfiguration, requestId);
        });

        for (auto streamInfoMap : muxedStreamInfoMaps)
        {
            processStreamInfos(streamInfoMap, playerConfiguration, requestId, 1);

            if (requestMode == 1) return;
        }
    }

    // Get adaptive stream infos
    if ((requestMode == 0) || (requestMode == 2) || (requestMode == 3))
    {
        connect(this, &YouTubeVideo::receivedAudioStreamInfo, [ = ](AudioStreamInfo *info, int reqId)
        {
            m_mediaStreamInfoCache[requestId].append(info);

            if (m_mediaStreamInfoCache[requestId].count() == count) onReceivedAllStreamInfos(playerConfiguration, requestId);
        });

        connect(this, &YouTubeVideo::receivedVideoStreamInfo, [ = ](VideoStreamInfo *info, int reqId)
        {
            m_mediaStreamInfoCache[requestId].append(info);

            if (m_mediaStreamInfoCache[requestId].count() == count) onReceivedAllStreamInfos(playerConfiguration, requestId);
        });

        for (auto streamInfoMap : adaptiveStreamInfoMaps)
        {
            processStreamInfos(streamInfoMap, playerConfiguration, requestId, 2);
        }
    }


    // Get dash manifest
    auto dashManifestUrl = playerConfiguration.dashManifestUrl();

    if (!dashManifestUrl.isEmpty())
    {
        // Only if video is a stream
        // TODO
    }
}

void YouTubeVideo::onReceivedAllStreamInfos(PlayerConfiguration playerConfiguration, int requestId)
{
    // Prepare stream info maps
    QList<MuxedStreamInfo *> muxedStreamInfos;
    QList<AudioStreamInfo *> audioStreamInfos;
    QList<VideoStreamInfo *> videoStreamInfos;

    for (auto info : m_mediaStreamInfoCache[requestId])
    {
        if (!info) continue;

        auto muxed = dynamic_cast<MuxedStreamInfo *>(info);
        auto audio = dynamic_cast<AudioStreamInfo *>(info);
        auto video = dynamic_cast<VideoStreamInfo *>(info);

        if (muxed) muxedStreamInfos.append(muxed);

        if (audio) audioStreamInfos.append(audio);

        if (video) videoStreamInfos.append(video);
    }

    emit receivedVideoMediaStreamInfos(
        new MediaStreamInfoSet(muxedStreamInfos, audioStreamInfos, videoStreamInfos,
                               playerConfiguration.hlsManifestUrl(), playerConfiguration.validUntil()), requestId);
}

void YouTubeVideo::processStreamInfos(QString streamInfoMap, PlayerConfiguration playerConfiguration, int requestId, int mode)
{
    QUrlQuery streamInfoMapQuery(streamInfoMap);

    // Extract info
    auto itag = streamInfoMapQuery.queryItemValue("itag").toInt();

    if (itag == 0)
    {
        auto type = QUrl::fromPercentEncoding(streamInfoMapQuery.queryItemValue("type").toUtf8());

        switch (mode)
        {
        case 1:
            emit receivedMuxedStreamInfo(nullptr, requestId);
            break;

        case 2:

            if (type.toLower().startsWith("audio/")) emit receivedAudioStreamInfo(nullptr, requestId);
            else emit receivedVideoStreamInfo(nullptr, requestId);
            break;
        }
        return;
    }

    auto url = QUrl(QUrl::fromPercentEncoding(streamInfoMapQuery.queryItemValue("url").toUtf8()));

    // Decipher signature if needed
    QString signature = streamInfoMapQuery.queryItemValue("s");

    if (!signature.isEmpty())
    {
        QString args = signature + " " + streamInfoMapQuery.toString();
        getCipherOperations(playerConfiguration.playerSourceUrl(), requestId, mode, args);
    }
    else
    {
        onReceivedStreamSignature(streamInfoMapQuery, url, requestId, mode);
    }
}

void YouTubeVideo::onReceivedStreamSignature(QUrlQuery streamInfoMapQuery, QUrl url, int requestId, int mode)
{
    // Try to extract content length, otherwise get it manually
    auto contentLength = YouTubeUtils::regexMatch(url.toString(), "clen=(\\d+)").toLongLong();

    if (contentLength <= 0)
    {
        // Send HEAD request and get content length
        auto reply = m_networkManager->head(QNetworkRequest(url));

        std::unique_ptr<QMetaObject::Connection> pconn{ new QMetaObject::Connection };
        QMetaObject::Connection& conn = *pconn;
        conn = connect(reply, &QNetworkReply::finished, [ = ]() {
            qint64 contentLength = reply->header(QNetworkRequest::ContentLengthHeader).toLongLong();

            // If content length is still not available - stream is gone or
            // faulty
            if (contentLength <= 0)
            {
                qDebug() << "CONTENT LENGTH IS 0:" << url;
                auto type = QUrl::fromPercentEncoding(streamInfoMapQuery.queryItemValue("type").toUtf8());

                switch (mode)
                {
                case 1:
                    emit receivedMuxedStreamInfo(nullptr, requestId);
                    break;

                case 2:

                    if (type.toLower().startsWith("audio/")) emit receivedAudioStreamInfo(nullptr, requestId);
                    else emit receivedVideoStreamInfo(nullptr, requestId);
                    break;
                }
                return;
            }

            switch (mode) {
            case 1:
                extractMuxedStreamProperties(streamInfoMapQuery, url, contentLength, requestId);
                break;

            case 2:
                extractAdaptiveStreamProperties(streamInfoMapQuery, url, contentLength, requestId);
                break;

            default: return;
            }
        });
    }
    else
    {
        switch (mode) {
        case 1:
            extractMuxedStreamProperties(streamInfoMapQuery, url, contentLength, requestId);
            break;

        case 2:
            extractAdaptiveStreamProperties(streamInfoMapQuery, url, contentLength, requestId);
            break;

        default: return;
        }
    }
}

void YouTubeVideo::onReceivedCipherOperations(QList<CipherOperation *>operations, QString args, int requestId, int mode)
{
    QString   signature = args.split(" ")[0];
    QUrlQuery streamInfoMapQuery(args.split(" ")[1]);
    QUrl url = QUrl(QUrl::fromPercentEncoding(streamInfoMapQuery.queryItemValue("url").toUtf8()));

    QList<CipherOperation *> cipherOperations = operations;

    // Decipher signature
    signature = CipherOperations::decipher(cipherOperations, signature);

    // Set the corresponding parameter in the URL
    auto signatureParameter = streamInfoMapQuery.queryItemValue("sp");

    if (signatureParameter.isEmpty()) signatureParameter = "signature";

    QUrlQuery query(url);
    query.addQueryItem(signatureParameter, signature);
    url.setQuery(query.query());


    onReceivedStreamSignature(streamInfoMapQuery, url, requestId, mode);
}

void YouTubeVideo::extractMuxedStreamProperties(QUrlQuery streamInfoMapQuery, QUrl url, qint64 contentLength, int requestId)
{
    auto type = QUrl::fromPercentEncoding(streamInfoMapQuery.queryItemValue("type").toUtf8());
    auto itag = streamInfoMapQuery.queryItemValue("itag").toInt();

    // Extract container
    auto containerRaw = type.left(type.indexOf(";"));

    containerRaw = containerRaw.right(containerRaw.length() - containerRaw.indexOf("/") - 1);
    auto container = Heuristics::containerFromString(containerRaw);

    // Extract audio encoding
    auto audioEncodingRaw = type.right(type.length() - type.indexOf("codecs=\"") - QString("codecs=\"").length());
    audioEncodingRaw = audioEncodingRaw.left(audioEncodingRaw.indexOf("\"")).split(",+").last();
    auto audioEncoding = Heuristics::audioEncodingFromString(audioEncodingRaw);

    // Extract video encoding
    auto videoEncodingRaw = type.right(type.length() - type.indexOf("codecs=\"") - QString("codecs=\"").length());
    videoEncodingRaw = videoEncodingRaw.left(videoEncodingRaw.indexOf("\"")).split(",+").first();
    auto videoEncoding = Heuristics::videoEncodingFromString(videoEncodingRaw);

    // Determine video quality from itag
    auto videoQuality = Heuristics::videoQualityFromItag(itag);

    // Determine video quality label from video quality
    auto videoQualityLabel = Heuristics::videoQualityToLabel(videoQuality);

    // Determine video resolution from video quality
    auto resolution = Heuristics::videoQualityToResolution(videoQuality);

    // Add to list
    emit receivedMuxedStreamInfo(new MuxedStreamInfo(
                                     itag, url, container, contentLength, audioEncoding,
                                     videoEncoding, videoQualityLabel, videoQuality, resolution,
                                     this), requestId);
}

void YouTubeVideo::extractAdaptiveStreamProperties(QUrlQuery streamInfoMapQuery, QUrl url, qint64 contentLength, int requestId)
{
    auto type    = QUrl::fromPercentEncoding(streamInfoMapQuery.queryItemValue("type").toUtf8());
    auto itag    = streamInfoMapQuery.queryItemValue("itag").toInt();
    auto bitrate = streamInfoMapQuery.queryItemValue("bitrate").toLongLong();

    // Extract container
    auto containerRaw = type.left(type.indexOf(";"));

    containerRaw = containerRaw.right(containerRaw.length() - containerRaw.indexOf("/") - 1);
    auto container = Heuristics::containerFromString(containerRaw);

    // If audio-only
    if (type.toLower().startsWith("audio/"))
    {
        // Extract audio encoding
        auto audioEncodingRaw = type.right(type.length() - type.indexOf("codecs=\"") - QString("codecs=\"").length());
        auto audioEncoding    = Heuristics::audioEncodingFromString(audioEncodingRaw);

        // Add to list
        emit receivedAudioStreamInfo(new AudioStreamInfo(itag, url, container, contentLength, bitrate, audioEncoding, this), requestId);
    }

    // If video-only
    else
    {
        // Extract video encoding
        auto videoEncodingRaw = type.right(type.length() - type.indexOf("codecs=\"") - QString("codecs=\"").length());
        auto videoEncoding    = !videoEncodingRaw.toLower().contains("unknown") ?
                                Heuristics::videoEncodingFromString(videoEncodingRaw) :
                                MediaStreamInfo::Av1;

        // Extract video quality label and video quality
        auto videoQualityLabel = streamInfoMapQuery.queryItemValue("quality_label");
        auto videoQuality      = Heuristics::videoQualityFromLabel(videoQualityLabel);

        // Extract resolution
        auto size   = streamInfoMapQuery.queryItemValue("size");
        auto width  = size.left(size.indexOf("x")).toInt();
        auto height = size.right(size.length() - size.indexOf("x") - 1).toInt();
        VideoResolution videoResolution(width, height);

        // Extract framerate
        auto framerate = streamInfoMapQuery.queryItemValue("fps").toInt();

        // Add to list
        emit receivedVideoStreamInfo(new VideoStreamInfo(
                                         itag, url, container, contentLength,
                                         bitrate, videoEncoding, videoQualityLabel,
                                         videoQuality, videoResolution, framerate,
                                         this), requestId);
    }
}

int YouTubeVideo::getVideoClosedCaptionTrackInfo(QString videoId)
{
}
