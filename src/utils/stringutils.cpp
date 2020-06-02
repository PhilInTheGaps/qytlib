#include "qytlib/utils/stringutils.h"

using namespace YouTube::Utils;

QString StringUtils::substringUntil(const QString &string, const QString &sub)
{
    auto index = string.indexOf(sub);
    return index < 0 ? string : string.left(string.indexOf(sub));
}

QString StringUtils::substringAfter(const QString& string, const QString &sub)
{
    auto index = string.lastIndexOf(sub);

    if (index < 0 || string.isEmpty() || string.endsWith(sub)) return "";

    return string.right(string.length() - index - sub.length());
}
