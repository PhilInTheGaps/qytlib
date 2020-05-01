#ifndef STREAMSCLIENT_H
#define STREAMSCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

#include "videos/videoid.h"
#include "videos/streams/istreaminfo.h"

#include "reverseengineering/responses/istreaminfoprovider.h"
#include "reverseengineering/responses/dashmanifest.h"
#include "reverseengineering/responses/embedpage.h"
#include "reverseengineering/responses/playersource.h"
#include "reverseengineering/responses/videoinforesponse.h"
#include "reverseengineering/cipher/icipheroperation.h"

#include "streammanifest.h"


Q_DECLARE_LOGGING_CATEGORY(ytStreamsClient)

namespace YouTube {
namespace Videos {
namespace Streams {

class StreamContext;
class LiveUrl;

class StreamsClient : public QObject
{
    Q_OBJECT
public:
    explicit StreamsClient(QNetworkAccessManager *networkManager, QObject *parent = nullptr)
        : QObject(parent), m_networkManager(networkManager) {}

    /// Gets the manifest that contains information about available streams in the specified video.
    StreamManifest* getManifest(const VideoId& videoId);

    /// Gets the HTTP Live Stream (HLS) manifest URL for the specified video (if it's a live video stream).
    /// The LiveUrl emits LiveUrl::ready() when the url can be read. (Through LiveUrl->url())
    LiveUrl *getLiveStreamUrl(const VideoId& videoId);

    /// Gets the actual stream which is identified by the specified metadata.
    void get(IStreamInfo *streamInfo);

private:
    QNetworkAccessManager *m_networkManager = nullptr;

    /// Try to load stream context from the embedded video page.
    /// If video is not allowed to be embedded, the stream context does
    /// not contain any streams.
    StreamContext* getStreamContextFromVideoInfo(const VideoId& videoId);

    /// Try to load stream context from the watch page. Used as a fallback
    /// if data can not be fetched from the embedded page.
    void getStreamContextFromWatchPage(const VideoId& videoId, StreamContext *streamContext);

    void loadStreamContext(StreamContext *streamContext, Responses::PlayerResponse *playerResponse, Responses::PlayerSource *playerSource, QVector<Responses::IStreamInfoProvider *> streams);

    void loadManifest(StreamManifest *streamManifest, StreamContext *streamContext);
};

class StreamContext : public Responses::IResponse
{
    Q_OBJECT

public:
    StreamContext(QObject *parent = nullptr) : Responses::IResponse(parent) {}

    void setReady() { emit ready(); }

    QVector<Responses::IStreamInfoProvider*> streamInfoProviders() const { return m_streamInfoProviders; }
    void setStreamInfoProviders(const QVector<Responses::IStreamInfoProvider*> providers);

    QVector<Cipher::ICipherOperation*> cipherOperations() const { return m_cipherOperations; }
    void setCipherOperations(QVector<Cipher::ICipherOperation*> operations);

private:
    QVector<Responses::IStreamInfoProvider*> m_streamInfoProviders;
    QVector<Cipher::ICipherOperation*> m_cipherOperations;
};

class LiveUrl : public Responses::IResponse
{
    Q_OBJECT

public:
    LiveUrl(QObject *parent = nullptr) : IResponse(parent) {}

    QString url() const { return m_url; }
    void setUrl(const QString& url) { m_url = url; }

    void setReady() { emit ready(); }

private:
    QString m_url;
};

}
}
}

#endif // STREAMSCLIENT_H
