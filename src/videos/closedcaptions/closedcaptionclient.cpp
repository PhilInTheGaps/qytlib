#include "qytlib/videos/closedcaptions/closedcaptionclient.h"
#include "qytlib/reverseengineering/responses/closedcaptiontrackresponse.h"

#include <QTextStream>

Q_LOGGING_CATEGORY(ytClosedCaptionClient, "yt.videos.cc.client");

using namespace YouTube::Videos::CC;

ClosedCaptionClient::ClosedCaptionClient(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent), m_networkManager(networkManager)
{

}

ClosedCaptionManifest *ClosedCaptionClient::getManifest(const YouTube::Videos::VideoId &videoId)
{
    qCDebug(ytClosedCaptionClient()) << "ClosedCaptionClient::getManifest(" << videoId << ")";
    return ClosedCaptionManifest::get(videoId, m_networkManager, this);
}

ClosedCaptionTrack *ClosedCaptionClient::get(ClosedCaptionTrackInfo *trackInfo)
{
    qCDebug(ytClosedCaptionClient()) << "ClosedCaptionClient::get()";

    if (!trackInfo)
    {
        qCWarning(ytClosedCaptionClient()) << "ClosedCaptionClient::get(): Error: TrackInfo is nullptr.";
        return nullptr;
    }

    auto *closedCaptionTrack = new ClosedCaptionTrack(this);
    auto *response = Responses::ClosedCaptionTrackResponse::get(m_networkManager, trackInfo->url(), this);

    connect(response, &Responses::ClosedCaptionTrackResponse::ready, closedCaptionTrack, [=]() {
        qCDebug(ytClosedCaptionClient()) << "ClosedCaptionClient::get(): Received ClosedCaptionTrackResponse";
        closedCaptionTrack->setCaptions(response->closedCaptions());
    });

    return closedCaptionTrack;
}

bool ClosedCaptionClient::writeToDevice(ClosedCaptionTrack *track, QIODevice *device)
{
    qCDebug(ytClosedCaptionClient()) << "ClosedCaptionClient::writeToDevice()";

    if (!track)
    {
        qCWarning(ytClosedCaptionClient()) << "Error: No ClosedCaptionTrack specified.";
        return false;
    }

    if (!device)
    {
        qCWarning(ytClosedCaptionClient()) << "Error: No QIODevice specified.";
        return false;
    }

    if (!device->isOpen() || !device->isWritable())
    {
        qCWarning(ytClosedCaptionClient()) << "Error: QIODevice is not opened for writing.";
        return false;
    }

    int lineNumber = 1;

    QTextStream out(device);

    for (const auto *caption : track->captions())
    {
        // Line number
        out << QString::number(lineNumber++) << "\n";

        // Time start --> time end
        out << QTime::fromMSecsSinceStartOfDay(caption->offset()).toString("hh:mm:ss,zzz");
        out << " --> ";
        out << QTime::fromMSecsSinceStartOfDay(caption->offset() + caption->duration()).toString("hh:mm:ss,zzz");
        out << "\n";

        // Actual text
        out << caption->text() << "\n";
    }

    if (out.status() != QTextStream::Ok)
    {
        qCWarning(ytClosedCaptionClient()) << "An error occurred while writing to the device:" << out.status();
        return false;
    }

    return true;
}
