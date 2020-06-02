#ifndef URLUTILS_H
#define URLUTILS_H

#include <QString>

namespace YouTube {
namespace Utils {

class UrlUtils
{
public:
    /// Change or add query parameter of url.
    static QString setQueryParameter(const QString& url, const QString& key, const QString& value);

    /// Change or add route parameter of url.
    /// Assembled url looks like this: url/key/value
    static QString setRouteParameter(const QString &url, const QString& key, const QString& value);

    /// Extract the stream signature from the url.
    static QString getSignatureFromUrl(const QString &url);

};

}
}



#endif // URLUTILS_H
