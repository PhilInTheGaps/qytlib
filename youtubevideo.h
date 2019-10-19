#ifndef YOUTUBEVIDEO_H
#define YOUTUBEVIDEO_H

#include <QObject>
#include <QUrlQuery>
#include <QNetworkAccessManager>

#include "lib/QGumboParser/QGumboParser/qgumbodocument.h"
#include "lib/QGumboParser/QGumboParser/qgumbonode.h"

#include "models/video.h"
#include "models/channel.h"
#include "models/mediastreams/mediastreaminfoset.h"
#include "cipheroperations/cipheroperation.h"
#include "internal/playerconfiguration.h"
#include "models/closedcaptions/closedcaptiontrackinfo.h"

class YouTubeVideo : public QObject
{
    Q_OBJECT
public:
    YouTubeVideo(QNetworkAccessManager *networkManager);

    /**
     * @brief Gets video information by ID,
     * receivedVideo() is emitted when done
     */
    int getVideo(QString videoId);

    /**
     * @brief Gets author channel information for given video,
     * receivedAuthorChannel() is emitted when done
     */
    int getVideoAuthorChannel(QString videoId);

    /**
     * @brief Gets a set of all available media stream infos for given video,
     * receivedVideoMediaStreamInfos() is emitted when done
     */
    int getVideoMediaStreamInfos(QString videoId, int requestMode = 0);

    /**
     * @brief Gets all available closed caption track infos for given video,
     * receivedVideoClosedCaptionTrackInfos() is emitted when done
     */
    int getVideoClosedCaptionTrackInfo(QString videoId);

signals:
    void receivedVideo(Video video, int requestId);
    void receivedAuthorChannel(Channel channel, int requestId);
    void receivedVideoMediaStreamInfos(MediaStreamInfoSet *mediaInfos, int requestId);
    void receivedVideoClosedCaptionTrackInfos(QList<ClosedCaptionTrackInfo> trackInfos, int requestId);

    void receivedVideoInfoMap(QUrlQuery infoMap, int requestId);
    void receivedVideoWatchPageHtml(QGumboNode htmlNode, int requestId);
    void receivedVideoEmbedPageHtml(QGumboNode htmlNode, int requestId);
    void receivedDashManifestXml(QString rawXml, int requestId);
    void receivedPlayerConfiguration(PlayerConfiguration configuration, int requestId, int requestMode);

    void receivedCipherOperations(QList<CipherOperation*> operations, QString args, int requestId, int mode);

    void receivedMuxedStreamInfo(MuxedStreamInfo *streamInfo, int requestId);
    void receivedAudioStreamInfo(AudioStreamInfo *streamInfo, int requestId);
    void receivedVideoStreamInfo(VideoStreamInfo *streamInfo, int requestId);

private:
    QNetworkAccessManager *m_networkManager;
    QMap<QString, QList<CipherOperation*>> m_cipherOperationsCache;
    QMap<int, QList<MediaStreamInfo*>> m_mediaStreamInfoCache;
    int m_requestCount = 0;

    int getVideoInfoMap(QString videoId);
    int getVideoWatchPageHtml(QString videoId);
    int getVideoEmbedPageHtml(QString videoId);
    int getDashManifestXml(QUrl url);
    int getPlayerConfiguration(QString videoId, int requestId, int requestMode);
    int getCipherOperations(QUrl playerSourceUrl, int requestId, int mode = 0, QString args = "");

    void processStreamInfos(QString streamInfoMap, PlayerConfiguration playerConfiguration, int requestId, int mode);
    void extractMuxedStreamProperties(QUrlQuery streamInfoMapQuery, QUrl url, qint64 contentLength, int requestId);
    void extractAdaptiveStreamProperties(QUrlQuery streamInfoMapQuery, QUrl url, qint64 contentLength, int requestId);

    void extractContentLength(QUrl);

private slots:
    void onReceivedPlayerConfiguration(PlayerConfiguration playerConfiguration, int requestId, int requestMode);
    void onReceivedStreamSignature(QUrlQuery streamInfoMapQuery, QUrl url, int requestId, int mode);
    void onReceivedCipherOperations(QList<CipherOperation*> operations, QString args, int requestId, int mode);
    void onReceivedAllStreamInfos(PlayerConfiguration playerConfiguration, int requestId);
};

#endif // YOUTUBEVIDEO_H
