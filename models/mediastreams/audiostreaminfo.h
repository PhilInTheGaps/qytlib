#ifndef AUDIOSTREAMINFO_H
#define AUDIOSTREAMINFO_H

#include "mediastreaminfo.h"

class AudioStreamInfo : public MediaStreamInfo
{
public:
    AudioStreamInfo(int itag, QUrl url, Container container, qint64 size, qint64 bitrate, AudioEncoding audioEncoding, QObject* parent)
        :   MediaStreamInfo(itag, url, container, size, parent),
            m_bitrate(bitrate), m_audioEncoding(audioEncoding) {}

    /**
     * @brief Bitrate (bits/s) of the associated stream.
     */
    qint64 bitrate() const { return m_bitrate; }

    /**
     * @brief Audio encoding of the associated stream
     */
    AudioEncoding audioEncoding() const { return m_audioEncoding; }

    QString toString() const { return QString("%1 (%2) [audio]").arg(m_itag).arg(QMetaEnum::fromType<Container>().valueToKey(m_container)); }

private:
    qint64 m_bitrate;
    AudioEncoding m_audioEncoding;
};

#endif // AUDIOSTREAMINFO_H
