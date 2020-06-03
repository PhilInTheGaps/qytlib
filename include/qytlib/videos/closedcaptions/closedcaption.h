#ifndef CLOSEDCAPTION_H
#define CLOSEDCAPTION_H

#include <QObject>

#include "closedcaptionpart.h"

namespace YouTube {
namespace Videos {
namespace CC {

/// Text that gets displayed at specific time during video playback, as part of a ClosedCaptionTrack
class ClosedCaption : public QObject
{
    Q_OBJECT
public:
    explicit ClosedCaption(const QString &text, qint64 offset, qint64 duration,
                           QList<ClosedCaptionPart*> parts, QObject *parent = nullptr)
        : QObject(parent), m_text(text), m_offset(offset), m_duration(duration), m_parts(parts)
    {
        for (auto *part : parts) part->setParent(this);
    }

    /// Text displayed by this caption.
    QString text() const { return m_text; }

    /// Time at which this caption starts being displayed.
    qint64 offset() const { return m_offset; }

    /// Duration this caption is displayed.
    qint64 duration() const { return m_duration; }

    /// Caption parts (usually individual words).
    /// May be empty because not all captions contain parts.
    const QList<ClosedCaptionPart*> parts() const { return m_parts; }

    /// Gets the caption part displayed at the specified point in time, relative to this caption's offset.
    /// Returns null if not found.
    /// Note that some captions may not have any parts at all.
    ClosedCaptionPart *getPartByTime(qint64 msecs) const
    {
        for (auto *part : qAsConst(m_parts))
        {
            if (part->offset() >= msecs) return part;
        }

        return nullptr;
    }

    inline operator QString() const { return text(); }

private:
    QString m_text;
    qint64 m_offset;
    qint64 m_duration;

    QList<ClosedCaptionPart*> m_parts;
};

}
}
}

#endif // CLOSEDCAPTION_H
