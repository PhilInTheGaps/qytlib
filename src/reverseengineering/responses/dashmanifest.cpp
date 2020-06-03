#include "qytlib/reverseengineering/responses/dashmanifest.h"
#include "qytlib/utils/urlutils.h"

#include <QNetworkReply>
#include <tinyxml2.h>

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
    qCDebug(ytDashManifest()) << "DashManifest::parse()";

    tinyxml2::XMLDocument doc;
    auto result = doc.Parse(raw);

    if (result != tinyxml2::XML_SUCCESS)
    {
        qCWarning(ytDashManifest()) << "Error: Could not parse raw xml.";
        emit ready();
        return;
    }

    auto *representation = doc.FirstChildElement("Representation");

    while (representation)
    {
        auto *streamInfo = new StreamInfoDM(
                    QString(representation->Attribute("id")).toInt(),
                    QString(representation->Attribute("bandwidth")).toLongLong(),
                    QString(representation->Attribute("codecs")),
                    QString(representation->Attribute("width")).toInt(),
                    QString(representation->Attribute("height")).toInt(),
                    QString(representation->Attribute("frameRate")).toInt(),
                    this);

        auto *initialization = representation->FirstChildElement("Initialization");

        if (initialization)
        {
            auto sourceUrl = QString(initialization->Attribute("sourceURL"));

            // If sourceURL contains "sq/" then remove streaminfo.
            // I don't know why, but YouTube Explode does it, so there must be a reason.
            if (sourceUrl.isEmpty() || sourceUrl.contains("sq/"))
            {
                qCDebug(ytDashManifest()) << " Destroying dash stream, it's invalid:" << sourceUrl;
                streamInfo->deleteLater();
            }
            else
            {
                auto *baseUrl = representation->FirstChildElement("BaseURL");

                if (baseUrl)
                {
                    streamInfo->setUrl(baseUrl->GetText());
                }
                else
                {
                    qCWarning(ytDashManifest()) << "DashManifest::parse(): Error: Could not read BaseURL XML element.";
                }

                auto *audioChannelConfig = representation->FirstChildElement("AudioChannelConfiguration");

                if (audioChannelConfig) streamInfo->setIsAudioOnly(true);

                qCDebug(ytDashManifest()) << "DashManifest::parse(): Adding new stream info" << streamInfo->codecs();
                m_streams.append(streamInfo);
            }
        }
        else
        {
            qCWarning(ytDashManifest()) << "DashManifest::parse(): Error, could not find \"Initialization\" XML element.";
            streamInfo->deleteLater();
        }

        representation = representation->NextSiblingElement("Representation");
    }

    qCDebug(ytDashManifest()) << "DashManifest::parse(): Found" << m_streams.length() << "dash streams.";

    emit ready();
}

StreamInfoDM::StreamInfoDM(const int &tag, const int &bitrate, const QString &codecs, const int &width,
                           const int &height, const int &framerate, QObject *parent)
    : IStreamInfoProvider(parent), m_tag(tag), m_bitrate(bitrate), m_codecs(codecs), m_videoWidth(width),
      m_videoHeight(height), m_framerate(framerate)
{

}
