#include "videoinforesponse.h"
#include "streaminfovir.h"
#include "utils/regexutils.h"

#include <QNetworkReply>
#include <QUrl>

Q_LOGGING_CATEGORY(ytVideoInfoResponse, "yt.response.videoinfo");

using namespace YouTube::Responses;

VideoInfoResponse::VideoInfoResponse(const QByteArray &raw, QObject *parent) : IResponse(parent)
{   
    parse(raw);
}

VideoInfoResponse *VideoInfoResponse::get(QNetworkAccessManager *networkManager, const YouTube::Videos::VideoId &videoId, const QString &sts, QObject *parent)
{
    qCDebug(ytVideoInfoResponse()) << "Getting info response for video" << videoId << "sts:" << sts;

    auto *response = new VideoInfoResponse(parent);
    auto eurl = QString("https://youtube.googleapis.com/v/%1").arg(videoId);
    auto url = QUrl(QString("https://youtube.com/get_video_info?video_id=%1&el=embedded&eurl=%2&hl=en&sts=%3")
                    .arg(videoId, QString(eurl), sts));

    auto *reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, response, [ = ]() {
        auto raw = reply->readAll();
        reply->deleteLater();

        response->parse(raw);

        if (!response->isVideoAvailable() || !response->playerResponse()->isVideoAvailable())
        {
            qCWarning(ytVideoInfoResponse()) << "Error: video" << videoId << "is not available!";
            return;
        }
    });

    return response;
}

void VideoInfoResponse::parse(const QByteArray &raw)
{
    m_root = QUrlQuery(raw);

    auto playerResponseRaw = QUrl::fromPercentEncoding(m_root.queryItemValue("player_response").toUtf8());
    auto playerResponseJson = QJsonDocument::fromJson(playerResponseRaw.toUtf8()).object();
    m_playerResponse = new PlayerResponse(playerResponseJson, this);

    loadMuxedStreams();
    loadAdaptiveStreams();

    emit ready();
}

void VideoInfoResponse::loadMuxedStreams()
{
    auto infos = m_root.queryItemValue("url_encoded_fmt_stream_map").split(',');

    for (const auto &info : qAsConst(infos))
    {
        m_muxedStreams.append(new StreamInfoVIR(QUrlQuery(info), this));
    }
}

void VideoInfoResponse::loadAdaptiveStreams()
{
    auto infos = m_root.queryItemValue("adaptive_fmts").split(',');

    for (const auto &info : qAsConst(infos))
    {
        m_adaptiveStreams.append(new StreamInfoVIR(QUrlQuery(info), this));
    }
}
