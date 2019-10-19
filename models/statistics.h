#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>

class Statistics
{
public:
    Statistics(qint64 viewCount, qint64 likeCount, qint64 dislikeCount) :
        m_viewCount(viewCount), m_likeCount(likeCount), m_dislikeCount(dislikeCount) {}

    /**
     * @brief View count.
     */
    qint64 viewCount() const { return m_viewCount; }

    /**
     * @brief Like count.
     */
    qint64 likeCount() const { return m_likeCount; }

    /**
     * @brief Dislike count.
     */
    qint64 dislikeCount() const { return m_dislikeCount; }

    /**
     * @brief Average user rating in stars (1 star to 5 stars).
     */
    qreal averageRating() const {
        if (m_likeCount + m_dislikeCount == 0) return 0;
        return 1 + 4.0 * m_likeCount / (m_likeCount + m_dislikeCount);
    }

private:
    qint64 m_viewCount, m_likeCount, m_dislikeCount;

};

#endif // STATISTICS_H
