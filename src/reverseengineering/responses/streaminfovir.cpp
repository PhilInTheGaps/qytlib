#include "qytlib/reverseengineering/responses/streaminfovir.h"
#include "qytlib/utils/regexutils.h"

using namespace YouTube::Responses;

qint64 StreamInfoVIR::contentLength() const
{
    auto length = m_root.queryItemValue("clen").toLong();

    if (length > 0) return length;

    return Utils::RegExUtils::regexMatch(url(), "clen=(\\d+)").toLong();
}

QString StreamInfoVIR::container() const
{
    return Utils::StringUtils::substringAfter(Utils::StringUtils::substringUntil(mimeType(), ";"), "/");
}

QString StreamInfoVIR::codecs() const
{
    return Utils::StringUtils::substringUntil(Utils::StringUtils::substringAfter(mimeType(), "codecs=\""), "\"");
}

int StreamInfoVIR::videoWidth() const
{
    return Utils::StringUtils::substringUntil(m_root.queryItemValue("size"), "x").toInt();
}

int StreamInfoVIR::videoHeight() const
{
    return Utils::StringUtils::substringAfter(m_root.queryItemValue("size"), "x").toInt();
}
