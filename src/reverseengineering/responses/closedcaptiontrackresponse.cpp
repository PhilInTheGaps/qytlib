#include "qytlib/reverseengineering/responses/closedcaptiontrackresponse.h"
#include "qytlib/videos/closedcaptions/closedcaptionpart.h"
#include "qytlib/utils/urlutils.h"

#include <QNetworkReply>
#include <tinyxml2.h>

using namespace YouTube::Responses;

Q_LOGGING_CATEGORY(ytClosedCaptionTrackResponse, "yt.responses.closedcaptiontrack");

ClosedCaptionTrackResponse *ClosedCaptionTrackResponse::get(QNetworkAccessManager *networkManager, const QString &url, QObject *parent)
{
    qCDebug(ytClosedCaptionTrackResponse()) << "ClosedCaptionTrackResponse::get()";

    auto *closedCaptionTrackResponse = new ClosedCaptionTrackResponse(parent);

    // Enforce known format
    auto finalUrl = Utils::UrlUtils::setQueryParameter(url, "format", "3");
    auto *reply = networkManager->get(QNetworkRequest(finalUrl));

    connect(reply, &QNetworkReply::finished, closedCaptionTrackResponse, [=]() {
        if (reply->error() != QNetworkReply::NoError)
        {
            qCWarning(ytClosedCaptionTrackResponse()) << "ClosedCaptionTrackResponse::get():"
                                                      << reply->error() << reply->errorString();
            reply->deleteLater();
        }

        closedCaptionTrackResponse->parse(reply->readAll());
        reply->deleteLater();
    });

    return closedCaptionTrackResponse;
}

void ClosedCaptionTrackResponse::parse(const QByteArray &raw)
{
    qCDebug(ytClosedCaptionTrackResponse()) << "ClosedCaptionTrackResponse::parse()";

    tinyxml2::XMLDocument doc;
    auto result = doc.Parse(raw);

    if (result != tinyxml2::XML_SUCCESS)
    {
        qCWarning(ytClosedCaptionTrackResponse()) << "Error: Could not parse raw xml.";
        emit ready();
        return;
    }

    auto *timedtext = doc.FirstChildElement("timedtext");

    if (timedtext)
    {
        auto *body = timedtext->FirstChildElement("body");

        if (body)
        {
            auto *p = body->FirstChildElement("p");

            if (!p)
            {
                qCWarning(ytClosedCaptionTrackResponse()) << "ClosedCaptionTrackResponse::parse(): Error: Did not find \"p\" XML element.";
            }

            while (p)
            {
                auto text = QString::fromUtf8(p->GetText());
                QList<Videos::CC::ClosedCaptionPart*> parts;

                auto *s = p->FirstChildElement("s");

                while (s)
                {
                    auto textPart = QString::fromUtf8(s->GetText());
                    text.append(textPart);
                    parts.append(new Videos::CC::ClosedCaptionPart(
                                     textPart,
                                     QString(s->Attribute("t")).toLongLong(),
                                     this));
                    s = s->NextSiblingElement("s");
                }

                m_closedCaptions.append(new Videos::CC::ClosedCaption(
                                            text,
                                            QString(p->Attribute("t")).toLongLong(),
                                            QString(p->Attribute("d")).toLongLong(),
                                            parts, this));
                p = p->NextSiblingElement("p");
            }
        }
        else
        {
            qCWarning(ytClosedCaptionTrackResponse()) << "ClosedCaptionTrackResponse::parse(): Error: Did not find \"body\" XML element.";
        }
    }
    else
    {
        qCWarning(ytClosedCaptionTrackResponse()) << "ClosedCaptionTrackResponse::parse(): Error: Did not find \"timedtext\" XML element.";
    }

    emit ready();
}
