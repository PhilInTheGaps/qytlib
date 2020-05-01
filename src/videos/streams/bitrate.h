#ifndef BITRATE_H
#define BITRATE_H

#include <QObject>

namespace YouTube {
namespace Videos {
namespace Streams {

class Bitrate : public QObject
{
    Q_OBJECT

public:
    explicit Bitrate(const qint64& bitsPerSecond, QObject *parent = nullptr) : QObject(parent), m_bitsPerSecond(bitsPerSecond) {}

    inline operator QString() const { return QString("%1 %2").arg(QString::number(largestWholeNumberValue()), largestWholeNumberSymbol()); }

    /// Bits per second.
    qint64 bitsPerSecond() const { return m_bitsPerSecond; }

    /// Kilobits per second.
    qreal kiloBitsPerSecond() const { return bitsPerSecond() / 1024.0; }

    /// Megabits per second.
    qreal megaBitsPerSecond() const { return kiloBitsPerSecond() / 1024.0; }

    /// Gigabits per second
    qreal gigaBitsPerSecond() const { return megaBitsPerSecond() / 1024.0; }

    QString largestWholeNumberSymbol() const;

    qreal largestWholeNumberValue() const;

private:
    qint64 m_bitsPerSecond;
};

inline bool operator==(const Bitrate& o1, const Bitrate& o2) { return o1.bitsPerSecond() == o2.bitsPerSecond(); }
inline bool operator!=(const Bitrate& o1, const Bitrate& o2) { return o1.bitsPerSecond() != o2.bitsPerSecond(); }
inline bool operator>(const Bitrate& o1, const Bitrate& o2)  { return o1.bitsPerSecond() > o2.bitsPerSecond();  }
inline bool operator<(const Bitrate& o1, const Bitrate& o2)  { return o1.bitsPerSecond() < o2.bitsPerSecond();  }

}
}
}

#endif // BITRATE_H
