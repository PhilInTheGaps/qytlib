#ifndef MEDIASTREAMINFOSET_H
#define MEDIASTREAMINFOSET_H

#include <QObject>
#include <QList>
#include <QDateTime>

#include "muxedstreaminfo.h"
#include "audiostreaminfo.h"
#include "videostreaminfo.h"

class MediaStreamInfoSet
{
public:
    MediaStreamInfoSet(QList<MuxedStreamInfo*> muxed,
                       QList<AudioStreamInfo*> audio,
                       QList<VideoStreamInfo*> video,
                       QUrl hlsLiveStreamUrl,
                       QDateTime validUntil):
    m_muxed(muxed), m_audio(audio), m_video(video),
      m_hlsLiveStreamUrl(hlsLiveStreamUrl), m_validUntil(validUntil) {}

    ~MediaStreamInfoSet()
    {
        for (auto info : m_muxed) if (info) info->deleteLater();
        for (auto info : m_audio) if (info) info->deleteLater();
        for (auto info : m_video) if (info) info->deleteLater();
    }

    /**
     * @brief Muxed streams.
     */
    QList<MuxedStreamInfo*> muxed() const { return m_muxed; }

    /**
     * @brief Audio-only streams.
     */
    QList<AudioStreamInfo*> audio() const { return m_audio; }

    /**
     * @brief Video-only streams.
     */
    QList<VideoStreamInfo*> video() const { return m_video; }

    /**
     * @brief Raw HTTP Live Streaming (HLS) URL to the m3u8 playlist.
     * Null if not a live stream.
     */
    QUrl hlsLiveStreamUrl() const { return m_hlsLiveStreamUrl; }

    /**
     * @brief Expiry date for this information.
     */
    QDateTime validUntil() const { return m_validUntil; }

private:
    QList<MuxedStreamInfo*> m_muxed;
    QList<AudioStreamInfo*> m_audio;
    QList<VideoStreamInfo*> m_video;

    QUrl m_hlsLiveStreamUrl;
    QDateTime m_validUntil;
};

#endif // MEDIASTREAMINFOSET_H
