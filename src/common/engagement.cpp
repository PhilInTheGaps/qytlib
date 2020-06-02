#include "qytlib/common/engagement.h"

using namespace YouTube::Common;

qreal Engagement::averageRating() const
{
    if (m_likeCount + m_dislikeCount != 0)
    {
        return 1 + 4.0 * m_likeCount / (m_likeCount + m_dislikeCount);
    }

    return 0;
}
