#include "qytlib/reverseengineering/responses/channelpage.h"

#include <QNetworkReply>
#include <qgumbodocument.h>
#include <qgumbonode.h>

Q_LOGGING_CATEGORY(ytChannelPage, "yt.responses.channelpage")

using namespace YouTube::Responses;

ChannelPage::ChannelPage(const QByteArray &raw, QObject *parent)
    : IResponse(parent)
{
    parse(raw);
}

ChannelPage *ChannelPage::get(QNetworkAccessManager *networkManager, const YouTube::Channels::ChannelId &id, QObject *parent)
{
    qCDebug(ytChannelPage()) << "ChannelPage::get(" << id << ")";

    auto url = QString("https://www.youtube.com/channel/%1?hl=en").arg(id);
    return ChannelPage::getFromUrl(networkManager, url, parent);
}

ChannelPage *ChannelPage::getByName(QNetworkAccessManager *networkManager, const QString &userName, QObject *parent)
{
    qCDebug(ytChannelPage()) << "ChannelPage::getByName(" << userName << ")";

    auto url = QString("https://www.youtube.com/user/%1?hl=en").arg(userName);
    return ChannelPage::getFromUrl(networkManager, url, parent);
}

ChannelPage *ChannelPage::getFromUrl(QNetworkAccessManager *networkManager, const QString &url, QObject *parent)
{
    auto *channelPage = new ChannelPage(parent);
    auto *reply = networkManager->get(QNetworkRequest(QUrl(url)));

    connect(reply, &QNetworkReply::finished, channelPage, [ = ]() {
        channelPage->parse(reply->readAll());

        if (!channelPage->isOk())
        {
            qCWarning(ytChannelPage()) << "ChannelPage::getByName() Error: Channel page is broken";
        }

        reply->deleteLater();
    });

    return channelPage;
}

void ChannelPage::parse(const QByteArray &data)
{
    auto doc = QGumboDocument::parse(data);
    auto root = doc.rootNode();

    const auto elements = root.getElementsByTagName(HtmlTag::META);

    for (const auto& element : elements)
    {
        auto property = element.getAttribute("property");

        if (property == "og:url")
        {
            m_isOk = true;
            m_url = element.getAttribute("content");
        }
        else if (property == "og:title")
        {
            m_title = element.getAttribute("content");
        }
        else if (property == "og:image")
        {
            m_logoUrl = element.getAttribute("content");
        }
    }

    emit ready();

    if (!isOk()) qCWarning(ytChannelPage()) << "ChannelPage::parse() Error: Could not parse ChannelPage.";
}
