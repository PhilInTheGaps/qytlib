#ifndef VIDEO_H
#define VIDEO_H

#include <QObject>
#include <QDateTime>

#include "models/thumbnailset.h"
#include "models/statistics.h"

class Video
{
public:
    Video(QString id, QString author, QDateTime uploadDate, QString title, QString description,
          ThumbnailSet thumbnails, qint64 duration, QStringList keywords, Statistics statistics) :
    m_id(id), m_author(author), m_uploadDate(uploadDate), m_title(title), m_description(description),
    m_thumbnails(thumbnails), m_duration(duration), m_keywords(keywords), m_statistics(statistics) {}

    /**
     * @brief ID of this video.
     */
    QString id() const { return m_id; }

    /**
     * @brief Author of this video.
     */
    QString author() const { return m_author; }

    /**
     * @brief Upload date of this video.
     */
    QDateTime uploadDate() const { return m_uploadDate; }

    /**
     * @brief Title of this video.
     */
    QString title() const { return m_title; }

    /**
     * @brief Description of this video.
     */
    QString description() const { return m_description; }

    /**
     * @brief Thumbnails of this video.
     */
    ThumbnailSet thumbnails() const { return m_thumbnails; }

    /**
     * @brief Duration of this video.
     */
    qint64 duration() const { return m_duration; }

    /**
     * @brief Search keywords of this video.
     */
    QStringList keywords() const { return m_keywords; }

    /**
     * @brief Statistics of this video.
     */
    Statistics statistics() const { return m_statistics; }

    QString toString() const { return m_title; }

private:
    QString m_id, m_author, m_title, m_description;
    QDateTime m_uploadDate;
    ThumbnailSet m_thumbnails;
    qint64 m_duration;
    QStringList m_keywords;
    Statistics m_statistics;
};

#endif // VIDEO_H
