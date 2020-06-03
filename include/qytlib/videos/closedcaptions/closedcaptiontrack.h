#ifndef CLOSEDCAPTIONTRACK_H
#define CLOSEDCAPTIONTRACK_H

#include <QObject>

#include "closedcaption.h"

namespace YouTube {
namespace Videos {
namespace CC {

/// Track that contains closed captions in a specific language.
class ClosedCaptionTrack : public QObject
{
    Q_OBJECT
public:
    explicit ClosedCaptionTrack(QObject *parent = nullptr) : QObject(parent) {}

    /// Closed captions.
    const QList<ClosedCaption*> captions() const { return m_captions; }
    void setCaptions(const QList<ClosedCaption*> &captions);

    /// Gets the caption displayed at the specified point in time (milliseconds).
    /// Returns nullptr if not found.
    ClosedCaption *getByTime(qint64 msecs) const;

signals:
    void ready();

private:
    QList<ClosedCaption*> m_captions;
};

}
}
}

#endif // CLOSEDCAPTIONTRACK_H
