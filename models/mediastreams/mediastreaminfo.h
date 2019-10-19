#ifndef MEDIASTREAMINFO_H
#define MEDIASTREAMINFO_H

#include <QMetaEnum>
#include <QObject>
#include <QUrl>

class MediaStreamInfo : public QObject
{
    Q_OBJECT
public:
    enum Container { Mp4, WebM, Tgpp };
    Q_ENUM(Container)

    enum AudioEncoding { Aac, Vorbis, Opus };
    Q_ENUM(AudioEncoding)

    enum VideoEncoding { Mp4V, H263, H264, H265, Vp8, Vp9, Av1 };
    Q_ENUM(VideoEncoding)

    enum VideoQuality { Low144, Low240, Medium360, Medium480,
                        High720, High1080, High1440, High2160,
                        High2880, High3072, High4320 };
    Q_ENUM(VideoQuality)

    explicit MediaStreamInfo(int itag, QUrl url, Container container, qint64 size, QObject *parent) : QObject(parent)
    {
        m_itag      = itag;
        m_url       = url;
        m_container = container;
        m_size      = size < 0 ? 0 : size;
    }

    /**
     * @brief Unique tag that identifies the properties of the associated stream.
     */
    int itag() const { return m_itag; }

    /**
     * @brief URL of the endpoint that serves the associated stream.
     */
    QUrl url() const { return m_url; }

    /**
     * @brief Container type of the associated stream.
     */
    Container container() const { return m_container; }

    /**
     * @brief Content length (bytes) of the associated stream.
     */
    qint64 size() const { return m_size; }

    QString toString() const { return QString("%1 (%2)").arg(m_itag).arg(QMetaEnum::fromType<Container>().valueToKey(m_container)); }

protected:
    int m_itag;
    qint64 m_size;
    QUrl m_url;
    Container m_container;
};

#endif // MEDIASTREAMINFO_H
