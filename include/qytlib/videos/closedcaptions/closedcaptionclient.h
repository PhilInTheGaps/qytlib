#ifndef CLOSEDCAPTIONCLIENT_H
#define CLOSEDCAPTIONCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QLoggingCategory>

#include "closedcaptionmanifest.h"
#include "closedcaptiontrack.h"
#include "qytlib/videos/videoid.h"

Q_DECLARE_LOGGING_CATEGORY(ytClosedCaptionClient)

namespace YouTube {
namespace Videos {
namespace CC {

/// Queries related to closed captions of YouTube videos.
class ClosedCaptionClient : public QObject
{
    Q_OBJECT
public:
    explicit ClosedCaptionClient(QNetworkAccessManager *networkManager, QObject *parent = nullptr);

    /// Gets the manifest that contains information about available closed caption tracks in the specified video.
    ClosedCaptionManifest *getManifest(const VideoId &videoId);

    /// Gets the actual closed caption track which is identified by the specified metadata.
    ClosedCaptionTrack *get(ClosedCaptionTrackInfo *trackInfo);

    /// Writes the actual closed caption track to a QIODevice.
    /// Returns true if successful, false if not (also prints error to qWarning()).
    /// Captions are written in the SRT file format.
    static bool writeToDevice(ClosedCaptionTrack *track, QIODevice *device);

private:
    QNetworkAccessManager *m_networkManager = nullptr;
};

}
}
}

#endif // CLOSEDCAPTIONCLIENT_H
