#ifndef VIDEOCLIENT_H
#define VIDEOCLIENT_H

#include "qytlib/videos/video.h"
#include "qytlib/videos/streams/streamsclient.h"
#include "qytlib/videos/closedcaptions/closedcaptionclient.h"
#include "qytlib/reverseengineering/responses/watchpage.h"
#include "qytlib/reverseengineering/responses/videoinforesponse.h"

#include <QNetworkAccessManager>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(ytVideoClient)

namespace YouTube {
namespace Videos {

class VideoClient : public QObject
{
    Q_OBJECT
public:
    explicit VideoClient(QNetworkAccessManager *networkManager, QObject *parent = nullptr);

    /// Returns a pointer to the StreamsClient which is responsible for
    /// loading streams.
    ///
    /// A stream could be audio, video or a muxed stream.
    Streams::StreamsClient* streams() const { return m_streamsClient; }

    /// Queries related to closed captions of YouTube videos.
    CC::ClosedCaptionClient* closedCaptions() const { return m_closedCaptionsClient; }

    /// Gets the metadata associated with the specified video.
    /// The Video object will emit a ready signal when data can be read.
    Video *getVideo(const VideoId& videoId);

private:
    QNetworkAccessManager *m_networkManager = nullptr;
    Streams::StreamsClient *m_streamsClient = nullptr;
    CC::ClosedCaptionClient *m_closedCaptionsClient = nullptr;
};

}
}

#endif // VIDEOCLIENT_H
