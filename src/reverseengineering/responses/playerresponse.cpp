#include "playerresponse.h"
#include "utils/regexutils.h"

using namespace YouTube::Responses;
using namespace YouTube::Utils;

PlayerResponse::PlayerResponse(const QJsonObject &root, QObject *parent)
    : IResponse(parent)
{
    parse(root);
}

void PlayerResponse::parse(const QJsonObject &root)
{
    m_root = root;
    loadMuxedStreams();
    loadAdaptiveStreams();
}

QStringList PlayerResponse::keywords() const
{
    QStringList keywords;
    auto keywordsJson = videoDetails()["keywords"].toArray();

    for (const auto& keyword : qAsConst(keywordsJson))
    {
        keywords.append(keyword.toString());
    }

    return keywords;
}

QString PlayerResponse::previewVideoId() const
{
    auto errorScreen = playabilityStatus()["errorScreen"].toObject();

    auto videoId = errorScreen["playerLegacyDesktopYpcTrailerRenderer"].toObject()["trailerVideoId"].toString();

    if (!videoId.isEmpty()) return videoId;

    // Second try
    QUrlQuery query(errorScreen["ypcTrailerRenderer"].toObject()["playerVars"].toString());

    return query.queryItemValue("video_id");
}

void PlayerResponse::loadMuxedStreams()
{
    auto formats = streamingData()["formats"].toArray();

    for (const auto& format : qAsConst(formats))
    {
        m_muxedStreams.append(new StreamInfoPR(format.toObject(), this));
    }
}

void PlayerResponse::loadAdaptiveStreams()
{
    auto formats = streamingData()["adaptiveFormats"].toArray();

    for (const auto& format : qAsConst(formats))
    {
        m_adaptiveStreams.append(new StreamInfoPR(format.toObject(), this));
    }
}

void PlayerResponse::loadClosedCaptionTracks()
{
    auto tracks = m_root["captions"].toObject()["playerCaptionsTracklistRenderer"].toObject()["captionTracks"].toArray();

    for (const auto& track : qAsConst(tracks))
    {
        m_closedCaptionTracks.append(new ClosedCaptionTrackInfoPR(track.toObject(), this));
    }
}

QString StreamInfoPR::url() const
{
    auto url = m_root["url"].toString();

    if (!url.isEmpty()) return url;

    return QUrl::fromPercentEncoding(cipher().queryItemValue("url").toUtf8());
}

qint64 StreamInfoPR::contentLength() const
{
    auto length = m_root["contentLength"].toString().toLong();

    return length > 0 ? length : RegExUtils::regexMatch(url(), "clen=(\\d+)").toLong();
}

QString StreamInfoPR::container() const
{
    return StringUtils::substringAfter(StringUtils::substringUntil(mimeType(), ";"), "/");
}

QString StreamInfoPR::codecs() const
{
    return StringUtils::substringUntil(StringUtils::substringAfter(mimeType(), "codecs=\""), "\"");
}

