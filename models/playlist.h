#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QList>

#include "statistics.h"
#include "video.h"

class Playlist
{
public:
    enum PlaylistType {
        Normal,
        VideoMix,
        ChannelVideoMix,
        ChannelVideos,
        PopularChannelVideos,
        MusicAlbum,
        LikedVideos,
        Favorites,
        WatchLater
    };

    Playlist(QString id, QString author, QString title, QString description, Statistics statistics, QList<Video> videos)
        : m_id(id), m_author(author), m_title(title), m_description(description), m_statistics(statistics), m_videos(videos) {}

    /**
     * @brief ID of this playlist.
     */
    QString id() const { return m_id; }

    /**
     * @brief Type of this playlist.
     */
    PlaylistType type() const { return m_type; }

    /**
     * @brief Author of this playlist.
     */
    QString author() const { return m_author; }

    /**
     * @brief Title of this playlist.
     */
    QString title() const { return m_title; }

    /**
     * @brief Description of this playlist.
     */
    QString description() const { return m_description; }

    /**
     * @brief Statistics of this playlist.
     */
    Statistics statistics() const { return m_statistics; }

    /**
     * @brief Collection of videos contained in this playlist.
     */
    QList<Video> videos() const { return m_videos; }

    QString toString() const { return m_title; }

protected:
    static PlaylistType getPlaylistType(QString id);

private:
    QString m_id, m_author, m_title, m_description;
    PlaylistType m_type;
    Statistics m_statistics;
    QList<Video> m_videos;
};

#endif // PLAYLIST_H
