#ifndef MUXEDSTREAMINFO_H
#define MUXEDSTREAMINFO_H

#include "mediastreaminfo.h"
#include "videoresolution.h"

class MuxedStreamInfo : public MediaStreamInfo
{
public:
    MuxedStreamInfo(int itag, QUrl url, Container container, qint64 size, AudioEncoding audioEncoding, VideoEncoding videoEncoding, QString videoQualityLabel,
                    VideoQuality videoQuality, VideoResolution resolution, QObject *parent)
        :   MediaStreamInfo(itag, url, container, size, parent), m_audioEncoding(audioEncoding),
            m_videoEncoding(videoEncoding), m_videoQualityLabel(videoQualityLabel),
            m_videoQuality(videoQuality), m_resolution(resolution) {}


    /**
     * @brief Audio encoding of the associated stream
     */
    AudioEncoding audioEncoding() const { return m_audioEncoding; }

    /**
     * @brief Video encoding of the associated stream
     */
    VideoEncoding videoEncoding() const { return m_videoEncoding; }

    /**
     * @brief Video quality label of the associated stream
     */
    QString videoQualityLabel() const { return m_videoQualityLabel; }

    /**
     * @brief Video quality of the associated stream
     */
    VideoQuality videoQuality() const { return m_videoQuality; }

    /**
     * @brief Video resolution of the associated stream
     */
    VideoResolution resolution() const { return m_resolution; }

    QString toString() const { return QString("%1 (%2) [muxed]").arg(m_itag).arg(QMetaEnum::fromType<Container>().valueToKey(m_container)); }

private:
    AudioEncoding m_audioEncoding;
    VideoEncoding m_videoEncoding;
    QString m_videoQualityLabel;
    VideoQuality m_videoQuality;
    VideoResolution m_resolution;
};

#endif // MUXEDSTREAMINFO_H
