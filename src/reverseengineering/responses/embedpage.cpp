#include "qytlib/reverseengineering/responses/embedpage.h"
#include "qytlib/utils/regexutils.h"

#include <qgumbodocument.h>
#include <qgumbonode.h>

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

Q_LOGGING_CATEGORY(ytEmbedPage, "yt.responses.embedpage")

using namespace YouTube::Responses;
using namespace YouTube::Utils;

EmbedPage::EmbedPage(const QByteArray &raw, QObject *parent) : IResponse(parent)
{
    parse(raw);
}

EmbedPage *EmbedPage::get(QNetworkAccessManager *networkManager, const YouTube::Videos::VideoId &videoId, QObject *parent)
{
    qCDebug(ytEmbedPage()) << "EmbedPage::get(" << videoId << ")";

    auto *embedPage = new EmbedPage(parent);
    auto url = QString("https://youtube.com/embed/%1?hl=en").arg(videoId);
    auto *reply = networkManager->get(QNetworkRequest(QUrl(url)));

    connect(reply, &QNetworkReply::finished, embedPage, [ = ]() {
        embedPage->parse(reply->readAll());
        reply->deleteLater();
    });

    return embedPage;
}

void EmbedPage::parse(const QByteArray &raw)
{
    auto doc = QGumboDocument::parse(raw);
    auto root = doc.rootNode();

    const auto scripts = root.getElementsByTagName(HtmlTag::SCRIPT);

    for (const auto& tag : scripts)
    {
        auto text = tag.innerText();
        auto match = RegExUtils::match(text, "yt\\.setConfig\\({.*'PLAYER_CONFIG':(.*})}\\);", 1);

        if (!match.isEmpty())
        {
            auto json = QJsonDocument::fromJson(match.toUtf8()).object();
            m_playerConfig = new PlayerConfig(json, this);

            emit ready();
            return;
        }
    }

    qCWarning(ytEmbedPage()) << raw;
    qCWarning(ytEmbedPage()) << "Error: Could not load PlayerConfig from EmbedPage";
}
