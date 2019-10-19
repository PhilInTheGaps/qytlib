#ifndef THUMBNAILSET_H
#define THUMBNAILSET_H

#include <QString>
#include <QUrl>

class ThumbnailSet
{
public:
    ThumbnailSet(QString videoId) : m_videoId(videoId) {}

    /**
     * @brief Low resolution thumbnail URL.
     */
    QUrl lowResUrl() const { return QUrl("https://img.youtube.com/vi/" + m_videoId + "/default.jpg"); }

    /**
     * @brief Medium resolution thumbnail URL.
     */
    QUrl mediumResUrl() const { return QUrl("https://img.youtube.com/vi/" + m_videoId + "/mqdefault.jpg"); }

    /**
     * @brief High resolution thumbnail URL.
     */
    QUrl highResUrl() const { return QUrl("https://img.youtube.com/vi/" + m_videoId + "/hqdefault.jpg"); }

    /**
     * @brief Standard resolution thumbnail URL. Not always available.
     */
    QUrl standardResUrl() const { return QUrl("https://img.youtube.com/vi/" + m_videoId + "/sddefault.jpg"); }

    /**
     * @brief Max resolution thumbnail URL. Not always available.
     */
    QUrl maxResUrl() const { return QUrl("https://img.youtube.com/vi/" + m_videoId + "/maxresdefault.jpg"); }

private:
    QString m_videoId;
};

#endif // THUMBNAILSET_H
