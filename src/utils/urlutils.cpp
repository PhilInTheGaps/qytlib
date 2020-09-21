#include "qytlib/utils/urlutils.h"
#include "qytlib/utils/regexutils.h"

#include <QRegularExpression>
#include <QUrl>
#include <QUrlQuery>

using namespace YouTube::Utils;

/// Change or add query parameter of url.
QString UrlUtils::setQueryParameter(const QString &url, const QString &key, const QString &value)
{
    QUrl qurl(url);

    QUrlQuery *query = nullptr;

    // Url has query
    if (qurl.hasQuery())
    {
        query = new QUrlQuery(qurl);

        if (query->hasQueryItem(key))
            query->removeQueryItem(key);
    }
    else
    {
        query = new QUrlQuery;
    }

    query->addQueryItem(key, value);
    qurl.setQuery(*query);

    delete query;
    return qurl.toString();
}

/// Change or add route parameter of url.
/// Assembled url looks like this: url/key/value
QString UrlUtils::setRouteParameter(const QString& url, const QString &key, const QString &value)
{
    // Find existing parameter
    auto expression = QString("/(%1/?.*?)(?:/|$)").arg(QRegularExpression::escape(key));
    auto group = RegExUtils::match(url, expression);
    auto temp = url;

    // Parameter already set
    if (!group.isEmpty())
    {
        auto index = temp.indexOf(group);

        // Remove existing value
        temp.remove(index, group.length());

        // Insert new value
        temp.insert(index, QString("%1/%2").arg(key, value));
        return temp;
    }

    // Parameter has not been set
    if (temp.endsWith('/')) temp.chop(1);

    return temp + '/' + key + '/' + value;
}

/// Extract the stream signature from the url.
QString UrlUtils::getSignatureFromUrl(const QString &url)
{
    return Utils::RegExUtils::match(url, "/s/(.*?)(?:/|$)");
}

