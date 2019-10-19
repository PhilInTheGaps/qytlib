#ifndef YOUTUBE_H
#define YOUTUBE_H

#include <QObject>
#include <QDataStream>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QUrl>
#include <QRegularExpression>

#include "youtubevideo.h"
#include "models/playlist.h"


class YouTube : public QObject
{
    Q_OBJECT

public:
    explicit YouTube(QObject *parent = nullptr);
    ~YouTube();

    /* --- Videos --- */

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
    int getVideoMediaStreamInfos(QString videoId);

    int getVideoAudioStreamInfo(QString videoId);

    /**
     * @brief Gets all available closed caption track infos for given video,
     * receivedVideoClosedCaptionTrackInfos() is emitted when done
     */
    int getVideoClosedCaptionTrackInfo(QString videoId);


    /* --- Playlist --- */

    /**
     * @brief Gets playlist information by ID.
     * The video list is truncated at given number of pages (1 page ≤ 200 videos).
     * receivedPlaylist() is emitted when done
     */
    void getPlaylist(QString playlistId, int maxPages);

    /**
     * @brief Gets playlist information by ID.
     * receivedPlaylist() is emitted when done
     */
    void getPlaylist(QString playlistId);


    /* --- Search --- */

    /**
     * @brief Searches videos using given query.
     * The video list is truncated at given number of pages (1 page ≤ 20 videos).
     * foundVideos() is emitted when done
     */
    void searchVideos(QString query, int maxPages);

    /**
     * @brief Searches videos using given query.
     * foundVideos() is emitted when done
     */
    void searchVideos(QString query);


    /* --- Channel --- */

    /**
     * @brief Gets channel ID by username.
     * receivedChannelId() is emitted when done
     */
    void getChannelId(QString username);

    /**
     * @brief Gets channel information by ID.
     * receivedChannel() is emitted when done
     */
    void getChannel(QString channelId);

    /**
     * @brief Gets videos uploaded by channel with given ID.
     * The video list is truncated at given number of pages (1 page ≤ 200 videos).
     * receivedChannelUploads() is emitted when done
     */
    void getChannelUploads(QString channelId, int maxPages);

    /**
     * @brief Gets videos uploaded by channel with given ID.
     * receivedChannelUploads() is emitted when done
     */
    void getChannelUploads(QString channelId);


    /* --- MediaStream --- */

    /**
     * @brief Gets the media stream associated with given metadata.
     * receivedMediaStream() is emitted when done
     */
    void getMediaStream(MediaStreamInfo *info);

    /**
     * @brief Downloads the stream associated with given metadata to the output stream.
     * finishedMediaStreamDownload() is emitted when done
     */
    void downloadMediaStream(MediaStreamInfo *info, QDataStream output, qreal *progress);

signals:
    void receivedVideo(Video video, int requestId);
    void receivedAuthorChannel(Channel channel, int requestId);
    void receivedVideoMediaStreamInfos(MediaStreamInfoSet *mediaInfos, int requestId);
    void receivedVideoClosedCaptionTrackInfos(QList<ClosedCaptionTrackInfo> trackInfos, int requestId);

    void receivedPlaylist(Playlist playlist, int requestId);

    void foundVideos(QList<Video> videos, int requestId);

    void receivedChannelId(QString channelId, int requestId);
    void receivedChannel(Channel channel, int requestId);
    void receivedChannelUploads(QList<Video> uploads, int requestId);

    void receivedMediaStream(/*MediaStream mediaStream,*/ int requestId);
    void finishedMediaStreamDownload(MediaStreamInfo *info, int requestId);

private:
    QNetworkAccessManager *m_networkManager;
    YouTubeVideo *m_video;

    int m_requestCount = 0;
};

#endif // YOUTUBE_H
