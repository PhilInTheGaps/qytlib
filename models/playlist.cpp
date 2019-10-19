#include "playlist.h"

Playlist::PlaylistType Playlist::getPlaylistType(QString id)
{
    if (id.isEmpty()) return Playlist::PlaylistType::Normal;

    if (id.startsWith("PL", Qt::CaseInsensitive)) return Playlist::PlaylistType::Normal;

    if (id.startsWith("RD", Qt::CaseInsensitive)) return Playlist::PlaylistType::VideoMix;

    if (id.startsWith("UL", Qt::CaseInsensitive)) return Playlist::PlaylistType::ChannelVideoMix;

    if (id.startsWith("UU", Qt::CaseInsensitive)) return Playlist::PlaylistType::ChannelVideos;

    if (id.startsWith("PU", Qt::CaseInsensitive)) return Playlist::PlaylistType::PopularChannelVideos;

    if (id.startsWith("OL", Qt::CaseInsensitive)) return Playlist::PlaylistType::MusicAlbum;

    if (id.startsWith("LL", Qt::CaseInsensitive)) return Playlist::PlaylistType::LikedVideos;

    if (id.startsWith("FL", Qt::CaseInsensitive)) return Playlist::PlaylistType::Favorites;

    if (id.startsWith("WL", Qt::CaseInsensitive)) return Playlist::PlaylistType::WatchLater;

    return Playlist::PlaylistType::Normal;
}
