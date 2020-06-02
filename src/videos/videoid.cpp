#include "qytlib/videos/videoid.h"
#include "qytlib/utils/regexutils.h"

Q_LOGGING_CATEGORY(ytVideoId, "yt.video.id")

using namespace YouTube::Videos;
using namespace YouTube::Utils;

VideoId::VideoId(const QString &idOrUrl)
{
    m_value = VideoId::normalize(idOrUrl);
}

bool VideoId::isValid(const QString &id)
{
    if (id.isEmpty()) return false;

    // Video IDs are always 11 characters
    if (id.length() != 11) return false;

    return !RegExUtils::isMatch(id, "[^0-9a-zA-Z_\\-]");
}

QString VideoId::normalize(const QString& idOrUrl)
{
    if (idOrUrl.isEmpty())
    {
        qCWarning(ytVideoId()) << "VideoId::normalize() Error: Video ID is empty";
        return "";
    }

    if (VideoId::isValid(idOrUrl)) return idOrUrl;

    auto regularMatch = RegExUtils::regexMatch(idOrUrl, "youtube\\..+?/watch.*?v=(.*?)(?:&|/|$)");
    if (!regularMatch.isEmpty() && VideoId::isValid(regularMatch)) return regularMatch;

    auto shortMatch = RegExUtils::regexMatch(idOrUrl, "youtu\\.be/(.*?)(?:\\?|&|/|$)");
    if (!shortMatch.isEmpty() && VideoId::isValid(shortMatch)) return shortMatch;

    auto embedMatch = RegExUtils::regexMatch(idOrUrl, "youtube\\..+?/embed/(.*?)(?:\\?|&|/|$)");
    if (!embedMatch.isEmpty() && VideoId::isValid(embedMatch)) return embedMatch;

    qCWarning(ytVideoId()) << "VideoId::normalize() Error: ID or URL" << idOrUrl << "is not valid.";

    return "";
}
