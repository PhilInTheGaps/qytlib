#ifndef ENGAGEMENT_H
#define ENGAGEMENT_H

#include <QObject>

namespace YouTube {
namespace Common {

class Engagement : public QObject
{
    Q_OBJECT

public:
    explicit Engagement(QObject *parent = nullptr) : QObject(parent) {}
    explicit Engagement(qint64 viewCount, qint64 likeCount, qint64 dislikeCount, QObject *parent = nullptr)
        : QObject(parent), m_viewCount(viewCount), m_likeCount(likeCount), m_dislikeCount(dislikeCount) {}

    operator const QString() const { return QString("%1 views | %2 likes | %3 dislikes").arg(m_viewCount).arg(m_likeCount).arg(m_dislikeCount); }

    /// View count.
    qint64 viewCount() const { return m_viewCount; }
    void setViewCount(const qint64& count) { m_viewCount = count; }

    /// Like count.
    qint64 likeCount() const { return m_likeCount; }
    void setLikeCount(const qint64& count) { m_likeCount = count; }

    /// Dislike count.
    qint64 dislikeCount() const { return m_dislikeCount; }
    void setDislikeCount(const qint64& count) { m_dislikeCount = count; }

    /// Average rating.
    qreal averageRating() const;

private:
    qint64 m_viewCount = 0;
    qint64 m_likeCount = 0;
    qint64 m_dislikeCount = 0;
};

}
}


#endif // ENGAGEMENT_H
