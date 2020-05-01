#ifndef VIDEO_H
#define VIDEO_H

#include "videoid.h"
#include "common/thumbnailset.h"
#include "common/engagement.h"

#include <QObject>
#include <QDate>

namespace YouTube {
namespace Videos {

/**
 * @brief The Video class contains meta data for a YouTube video such as:
 *
 * - Title
 * - Author
 * - Description
 * - Upload Date
 * - Duration
 * - Keywords
 *
 * Video thumbnails can be accessed through thumbnails().
 * Views, likes, dislikes and average rating can be accessed through engagement().
 */
class Video : public QObject
{
    Q_OBJECT

public:
    explicit Video(const VideoId& id, QObject *parent = nullptr) : QObject(parent), m_id(id) {}

    operator const QString() const { return QString("Video (%1 - %2)").arg(m_title, m_author); }

    /// Video ID.
    VideoId id() const { return m_id; }

    /// Video URL.
    QString url() const { return QString("https://www.youtube.com/watch?v=%1").arg(m_id); }

    /// Video title.
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    QString title() const { return m_title; }
    void setTitle(const QString& title) { m_title = title; emit titleChanged(); }

    /// Video author.
    Q_PROPERTY(QString author READ author WRITE setAuthor NOTIFY authorChanged)
    QString author() const { return m_author; }
    void setAuthor(const QString& author) { m_author = author; emit authorChanged(); }

    /// Video upload date.
    Q_PROPERTY(QDate uploadDate READ uploadDate WRITE setUploadDate NOTIFY uploadDateChanged)
    QDate uploadDate() const { return m_uploadDate; }
    void setUploadDate(const QDate& date) { m_uploadDate = date; emit uploadDateChanged(); }

    /// Video description.
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    QString description() const { return m_description; }
    void setDescription(const QString& description) { m_description = description; emit descriptionChanged(); }

    /// Duration of the video.
    Q_PROPERTY(qint64 duration READ duration WRITE setDuration NOTIFY durationChanged)
    qint64 duration() const { return m_duration; }
    void setDuration(const qint64& duration) { m_duration = duration; emit durationChanged(); }

    /// Available thumbnails for this video.
    Q_PROPERTY(ThumbnailSet* thumbnails READ thumbnails WRITE setThumbnails NOTIFY thumbnailsChanged)
    ThumbnailSet* thumbnails() const { return m_thumbnails; }
    void setThumbnails(ThumbnailSet *thumbnails) { m_thumbnails = thumbnails; m_thumbnails->setParent(this), emit thumbnailsChanged(); }

    /// Search keywords used for this video.
    Q_PROPERTY(QStringList keywords READ keywords WRITE setKeywords NOTIFY keywordsChanged)
    QStringList keywords() const { return m_keywords; }
    void setKeywords(const QStringList& keywords) { m_keywords = keywords; emit keywordsChanged(); }

    /// Engagement statistics for this video.
    Q_PROPERTY(Common::Engagement* engagement READ engagement WRITE setEngagement NOTIFY engagementChanged)
    Common::Engagement* engagement() const { return m_engagement; }
    void setEngagement(Common::Engagement *engagement) { m_engagement = engagement; m_engagement->setParent(this); emit engagementChanged(); }

    /// Is video ready to be read
    Q_PROPERTY(bool isReady READ isReady WRITE setIsReady NOTIFY isReadyChanged)
    bool isReady() const { return m_isReady; }
    void setIsReady(const bool& isReady) { m_isReady = isReady; emit isReadyChanged(); if (m_isReady) emit ready(); }

signals:
    /// Emitted when video data has finished loading and can be read.
    void ready();
    void isReadyChanged();

    void titleChanged();
    void authorChanged();
    void uploadDateChanged();
    void descriptionChanged();
    void durationChanged();
    void thumbnailsChanged();
    void keywordsChanged();
    void engagementChanged();

private:
    VideoId m_id;
    bool m_isReady = false;

    QString m_title;
    QString m_author;
    QString m_description;
    qint64 m_duration;

    QDate m_uploadDate;
    ThumbnailSet *m_thumbnails = nullptr;
    QStringList m_keywords;
    Common::Engagement *m_engagement = nullptr;
};

}
}

#endif // VIDEO_H
