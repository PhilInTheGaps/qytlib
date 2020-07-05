#include "qytlib/reverseengineering/responses/watchpage.h"

#include <QNetworkReply>

#include <qgumbodocument.h>
#include <qgumbonode.h>

#include "qytlib/reverseengineering/responses/videoinforesponse.h"
#include "qytlib/utils/regexutils.h"

Q_LOGGING_CATEGORY(ytWatchPage, "yt.responses.watchpage")

using namespace YouTube::Responses;

WatchPage::WatchPage(const QByteArray &raw, QObject *parent)
    : QObject(parent)
{
    parse(raw);
}

WatchPage *WatchPage::get(QNetworkAccessManager *networkManager, const YouTube::Videos::VideoId &videoId, QObject *parent)
{
    qCDebug(ytWatchPage()) << "get(" << videoId << ")";

    auto *watchPage = new WatchPage(parent);
    auto url = QString("https://youtube.com/watch?v=%1&bpctr=9999999999&hl=en").arg(videoId);
    auto *reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, watchPage, [ = ]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(ytWatchPage()) << reply->error() << reply->errorString();
            reply->deleteLater();
            return;
        }

        watchPage->parse(reply->readAll());
        reply->deleteLater();

        if (!watchPage->isOk())
            qCWarning(ytWatchPage()) << "Error: Could not parse WatchPage, something went wrong.";
    });

    return watchPage;
}

void WatchPage::parse(const QByteArray &raw)
{
    qCDebug(ytWatchPage()) << "parse()";
    qCDebug(ytWatchPage()) << raw;

    auto doc = QGumboDocument::parse(raw);
    auto root = doc.rootNode();

    // Is Ok?
    auto player = root.getElementById("player");
    m_isOk = player.size() == 1;

    if (!m_isOk) qCWarning(ytWatchPage()) << raw;

    // Is video available?
    auto meta = root.getElementsByTagName(HtmlTag::META);

    for (const auto element : meta)
    {
        if (element.getAttribute("property") == "og:url")
        {
            m_isVideoAvailable = true;
            break;
        }
    }

    // Likes
    auto likeLabel = Utils::RegExUtils::regexMatch(raw, "\"label\"\\s*:\\s*\"([\\d,\\.]+) likes", 1);
    likeLabel.replace(',', "").replace(QRegularExpression("\\D"), ""); // Strip non digits
    m_videoLikeCount = likeLabel.toLong();

    // Dislikes
    auto dislikeLabel = Utils::RegExUtils::regexMatch(raw, "\"label\"\\s*:\\s*\"([\\d,\\.]+) dislikes");
    dislikeLabel.replace(',', "").replace(QRegularExpression("\\D"), ""); // Strip non digits
    m_videoDislikeCount = dislikeLabel.toLong();

    // PlayerConfig
    auto jsonRaw = Utils::RegExUtils::regexMatch(raw, "ytplayer\\.config = (\\{.*\\})\\;yt");
    auto json = QJsonDocument::fromJson(jsonRaw.toUtf8()).object();
    m_playerConfig = new PlayerConfig(json, this);

    emit ready();
}
