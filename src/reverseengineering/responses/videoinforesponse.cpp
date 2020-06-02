#include "qytlib/reverseengineering/responses/videoinforesponse.h"
#include "qytlib/reverseengineering/responses/streaminfovir.h"
#include "qytlib/utils/regexutils.h"

#include <QNetworkReply>
#include <QUrl>

Q_LOGGING_CATEGORY(ytVideoInfoResponse, "yt.response.videoinfo");

using namespace YouTube::Responses;

VideoInfoResponse *VideoInfoResponse::get(QNetworkAccessManager *networkManager, const YouTube::Videos::VideoId &videoId, const QString &sts, QObject *parent)
{
    qCDebug(ytVideoInfoResponse()) << "Getting info response for video" << videoId << "sts:" << sts;

    auto *response = new VideoInfoResponse(parent);
    auto eurl = QString("https://youtube.googleapis.com/v/%1").arg(videoId);
    auto url = QUrl(QString("https://youtube.com/get_video_info?video_id=%1&el=embedded&eurl=%2&hl=en&sts=%3")
                    .arg(videoId, QString(eurl), sts));

    auto *reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, response, [ = ]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(ytVideoInfoResponse()) << "VideoInfoResponse::get():" << reply->error() << reply->errorString();
            reply->deleteLater();
            return;
        }

        auto raw = reply->readAll();
        reply->deleteLater();
        response->parse(raw);
    });

    return response;
}

void VideoInfoResponse::parse(const QByteArray &raw)
{
    m_root = QUrlQuery(raw);

    // We need to replace '+' with spaces manually.
    // All actual plus signs are percent encoded, so we don't replace them by accident.
    auto playerResponseEncoded = m_root.queryItemValue("player_response").replace('+', ' ').toUtf8();
    auto playerResponseRaw = QUrl::fromPercentEncoding(playerResponseEncoded);
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
