#include "dashmanifest.h"
#include "utils/urlutils.h"

#include <QDebug>
#include <QNetworkReply>
#include <QXmlStreamReader>

Q_LOGGING_CATEGORY(ytDashManifest, "yt.responses.dashmanifest")

using namespace YouTube::Responses;

/// Get DashManifest from url
DashManifest *DashManifest::get(QNetworkAccessManager *networkManager, const QString &url, QObject *parent)
{
    qCDebug(ytDashManifest()) << "DashManifest::get(dashManifestUrl)";

    auto *dashManifest = new DashManifest(parent);
    auto *reply = networkManager->get(QNetworkRequest(url));

    connect(reply, &QNetworkReply::finished, dashManifest, [ = ]() {
        dashManifest->parse(reply->readAll());
        reply->deleteLater();
    });

    return dashManifest;
}

/// Get the DashManifest from url and decipher the signature
DashManifest *DashManifest::get(QNetworkAccessManager *networkManager, const QString &url, QVector<YouTube::Cipher::ICipherOperation *> cipherOperations, QObject *parent)
{
    qCDebug(ytDashManifest()) << "DashManifest::get(dashManifestUrl, cipherOperations)";

    auto dashManifestUrl = url;
    auto signature = Utils::UrlUtils::getSignatureFromUrl(dashManifestUrl);

    if (!signature.isEmpty())
    {
        signature = Cipher::Utils::decipher(cipherOperations, signature);
        dashManifestUrl = Utils::UrlUtils::setRouteParameter(dashManifestUrl, "signature", signature);
    }

    return DashManifest::get(networkManager, dashManifestUrl, parent);
}

void DashManifest::parse(const QByteArray &raw)
{
    QXmlStreamReader xml(raw);

    auto streamIndex = -1;
    StreamInfoDM *streamInfo = nullptr;

    while (!xml.atEnd())
    {
        auto tokenType = xml.readNext();

        if (tokenType == QXmlStreamReader::StartElement)
        {
            // Find element "Representation" and add StreamInfo
            if (xml.name() == "Representation")
            {
                auto id = xml.attributes().value("id").toInt();
                auto bitrate = xml.attributes().value("bandwidth").toLong();
                auto codecs = xml.attributes().value("codecs").toString();
                auto width = xml.attributes().value("width").toInt();
                auto height = xml.attributes().value("height").toInt();
                auto framerate = xml.attributes().value("frameRate").toInt();

                m_streams.append(new StreamInfoDM(id, bitrate, codecs, width, height, framerate, this));
                streamInfo = qobject_cast<StreamInfoDM*>(m_streams[++streamIndex]);

                qDebug() << "Adding new stream info" << streamIndex << streamInfo->codecs();
            }
            else if (streamInfo)
            {
                if (xml.name() == "Initialization")
                {
                    auto sourceUrl = xml.attributes().value("sourceURL");

                    // If sourceURL contains "sq/" then remove streaminfo.
                    // I don't know why, but YouTube Explode does it, so there must be a reason.
                    if (sourceUrl.isEmpty() || sourceUrl.contains("sq/"))
                    {
                        qDebug() << " Removing dash stream, it's invalid:" << sourceUrl << streamIndex;
                        m_streams.takeAt(streamIndex--)->deleteLater();
                        streamInfo = nullptr;
                    }
                }
                else if (xml.name() == "BaseURL")
                {
                    streamInfo->setUrl(xml.readElementText());
                }
                else if (xml.name() == "AudioChannelConfiguration")
                {
                    streamInfo->setIsAudioOnly(true);
                }
            }
        }
        else if (tokenType == QXmlStreamReader::EndElement)
        {
            if (xml.name() == "Representation")
            {
                streamInfo = nullptr;
            }
        }
    }

    qDebug() << "FOUND" << m_streams.length() << "DASH STREAMS";

    emit ready();
}

StreamInfoDM::StreamInfoDM(const int &tag, const int &bitrate, const QString &codecs, const int &width,
                           const int &height, const int &framerate, QObject *parent)
    : IStreamInfoProvider(parent), m_tag(tag), m_bitrate(bitrate), m_codecs(codecs), m_videoWidth(width),
      m_videoHeight(height), m_framerate(framerate)
{

}
