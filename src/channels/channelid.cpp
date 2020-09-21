#include "qytlib/channels/channelid.h"
#include "qytlib/utils/regexutils.h"

Q_LOGGING_CATEGORY(ytChannelId, "yt.channels.id")

using namespace YouTube::Channels;
using namespace YouTube::Utils;

ChannelId::ChannelId(const QString &idOrUrl)
{
    m_value = ChannelId::normalize(idOrUrl);
}

bool ChannelId::isValid(const QString &id)
{
    if (id.isEmpty()) return false;

    // Channel IDs start with "UC"
    if (!id.startsWith("UC")) return false;

    // Channel IDs are always 24 characters
    if (id.length() != 24) return false;

    return !RegExUtils::isMatch(id, "[^0-9a-zA-Z_\\-]");
}

QString ChannelId::normalize(const QString &idOrUrl)
{
    if (idOrUrl.isEmpty())
    {
        qCWarning(ytChannelId()) << "ChannelId::normalize() Error: Channel ID is empty";
        return QStringLiteral("");
    }

    if (ChannelId::isValid(idOrUrl)) return idOrUrl;

    auto regularMatch = RegExUtils::match(idOrUrl, "youtube\\..+?/channel/(.*?)(?:\\?|&|/|$)");
    if (!regularMatch.isEmpty() && ChannelId::isValid(regularMatch)) return regularMatch;

    qCWarning(ytChannelId()) << "ChannelId::normalize() Error: ID or URL" << idOrUrl << "is not valid.";

    return QStringLiteral("");
}
