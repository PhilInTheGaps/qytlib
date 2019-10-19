#ifndef VIDEOSTREAMINFO_H
#define VIDEOSTREAMINFO_H

#include "mediastreaminfo.h"
#include "videoresolution.h"

class VideoStreamInfo : public MediaStreamInfo
{
public:
    VideoStreamInfo(int itag, QUrl url, Container container, qint64 size, qint64 bitrate,
                    VideoEncoding videoEncoding, QString videoQualityLabel, VideoQuality videoQuality,
                    VideoResolution videoResolution, int framerate, QObject* parent)
        :   MediaStreamInfo(itag, url, container, size, parent),
            m_bitrate(bitrate), m_videoEncoding(videoEncoding), m_videoQualityLabel(videoQualityLabel),
            m_videoQuality(videoQuality), m_resolution(videoResolution), m_framerate(framerate) {}

    /**
     * @brief Bitrate (bits/s) of the associated stream
     */
    qint64 bitrate() const { return m_bitrate; }

    /**
     * @brief Video encoding of the associated stream.
     */
    VideoEncoding videoEncoding() const { return m_videoEncoding; }

    /**
     * @brief Video quality label of the associated stream
     */
    QString videoQualityLabel() const { return m_videoQualityLabel; }

    /**
     * @brief Video quality of the associated stream.
     */
    VideoQuality videoQuality() const { return m_videoQuality; }

    /**
     * @brief Video resolution of the associated stream.
     */
    VideoResolution resolution() const { return m_resolution; }

    /**
     * @brief Video framerate (FPS) of the associated stream.
     */
    int framerate() const { return m_framerate; }

    QString toString() const { return QString("%1 (%2) [video]").arg(m_itag).arg(QMetaEnum::fromType<Container>().valueToKey(m_container)); }

private:
    qint64 m_bitrate;
    VideoEncoding m_videoEncoding;
    QString m_videoQualityLabel;
    VideoQuality m_videoQuality;
    VideoResolution m_resolution;
    int m_framerate;
};

#endif // VIDEOSTREAMINFO_H
