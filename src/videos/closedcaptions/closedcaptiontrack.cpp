#include "qytlib/videos/closedcaptions/closedcaptiontrack.h"

using namespace YouTube::Videos::CC;

ClosedCaption *ClosedCaptionTrack::getByTime(qint64 msecs) const
{
    for (auto *caption : m_captions)
    {
        if (msecs >= caption->offset() && msecs <= caption->offset() + caption->duration())
            return caption;
    }

    return nullptr;
}

void ClosedCaptionTrack::setCaptions(const QList<ClosedCaption *> &captions)
{
    m_captions = captions;

    for (auto *caption : qAsConst(m_captions))
        caption->setParent(this);

    emit ready();
}
