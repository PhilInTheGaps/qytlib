#include "qytlib/channels/username.h"
#include "qytlib/utils/regexutils.h"

Q_LOGGING_CATEGORY(ytUserName, "yt.channels.username")

using namespace YouTube::Channels;
using namespace YouTube::Utils;

UserName::UserName(const QString& nameOrUrl)
{
    m_value = UserName::normalize(nameOrUrl);
}

bool UserName::isValid(const QString &name)
{
    if (name.isEmpty()) return false;

    // Usernames can not be longer than 20 characters
    if (name.length() > 20) return false;

    return !RegExUtils::isMatch(name, "[^0-9a-zA-Z]");
}

QString UserName::normalize(const QString &nameOrUrl)
{
    if (nameOrUrl.isEmpty())
    {
        qCWarning(ytUserName()) << "UserName::normalize() Error: UserName is empty";
        return QStringLiteral("");
    }

    if (UserName::isValid(nameOrUrl)) return nameOrUrl;

    auto regularMatch = RegExUtils::regexMatch(nameOrUrl, "youtube\\..+?/user/(.*?)(?:\\?|&|/|$)");
    if (!regularMatch.isEmpty() && UserName::isValid(regularMatch)) return regularMatch;

    qCWarning(ytUserName()) << "UserName::normalize() Error: Name or URL" << nameOrUrl << "is not valid.";
    return QStringLiteral("");
}
