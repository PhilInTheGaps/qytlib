#ifndef CLOSEDCAPTIONPART_H
#define CLOSEDCAPTIONPART_H

#include <QObject>

namespace YouTube {
namespace Videos {
namespace CC {

/// Part of a closed caption (usually a single word).
class ClosedCaptionPart : public QObject
{
    Q_OBJECT
public:
    explicit ClosedCaptionPart(const QString &text, qint64 offset, QObject *parent = nullptr)
        : QObject(parent), m_text(text), m_offset(offset) {}

    /// Text displayed by this caption part.
    QString text() const { return m_text; }

    /// Time at which this caption part starts being displayed (relative to the caption's own offset).
    qint64 offset() const { return m_offset; }

    inline operator QString() const { return text(); }

private:
    QString m_text;
    qint64 m_offset;
};

}
}
}

#endif // CLOSEDCAPTIONPART_H
