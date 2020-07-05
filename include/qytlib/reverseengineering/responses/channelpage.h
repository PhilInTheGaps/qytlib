#ifndef CHANNELPAGE_H
#define CHANNELPAGE_H

#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include "qytlib/reverseengineering/responses/iresponse.h"
#include "qytlib/utils/stringutils.h"
#include "qytlib/channels/channelid.h"

Q_DECLARE_LOGGING_CATEGORY(ytChannelPage)

namespace YouTube {
namespace Responses {

class ChannelPage : public IResponse
{
    Q_OBJECT
public:
    ChannelPage(QObject *parent = nullptr) : IResponse(parent) {}
    ChannelPage(const QByteArray& raw, QObject *parent = nullptr);

    static ChannelPage* get(QNetworkAccessManager *networkManager, const Channels::ChannelId& id, QObject *parent = nullptr);

    static ChannelPage* getByName(QNetworkAccessManager *networkManager, const QString& userName, QObject *parent = nullptr);

    void parse(const QByteArray& data);

    bool isOk() const { return m_isOk; }

    QString url() const { return m_url; }

    QString id() const { return Utils::StringUtils::substringAfter(url(), "channel/"); }

    QString title() const { return m_title; }

    QString logoUrl() const { return m_logoUrl; }

private:
    static ChannelPage *getFromUrl(QNetworkAccessManager *networkManager, const QString& url, QObject *parent = nullptr);

    bool m_isOk = false;
    QString m_url;
    QString m_title;
    QString m_logoUrl;
};

}
}

#endif // CHANNELPAGE_H
