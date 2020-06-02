#ifndef FILESIZE_H
#define FILESIZE_H

#include <QObject>

namespace YouTube {
namespace Videos {
namespace Streams {

class FileSize : public QObject
{
    Q_OBJECT

public:
    FileSize(const qint64& totalBytes, QObject *parent = nullptr) : QObject(parent), m_totalBytes(totalBytes) {}

    inline operator QString() const { return QString("%1 %2").arg(QString::number(largestWholeNumberValue()), largestWholeNumberSymbol()); }

    /// Total bytes.
    qint64 totalBytes() const { return m_totalBytes; }

    /// Total kilobytes.
    qreal totalKiloBytes() const { return m_totalBytes / 1024.0; }

    /// Total megabytes.
    qreal totalMegaBytes() const { return totalKiloBytes() / 1024.0; }

    /// Total gigabytes.
    qreal totalGigaBytes() const { return totalMegaBytes() / 1024.0; }

    QString largestWholeNumberSymbol() const;

    qreal largestWholeNumberValue() const;

private:
    qint64 m_totalBytes = 0;
};

inline bool operator==(const FileSize& o1, const FileSize& o2) { return o1.totalBytes() == o2.totalBytes(); }
inline bool operator!=(const FileSize& o1, const FileSize& o2) { return o1.totalBytes() != o2.totalBytes(); }
inline bool operator<(const FileSize& o1, const FileSize& o2)  { return o1.totalBytes() < o2.totalBytes();  }
inline bool operator>(const FileSize& o1, const FileSize& o2)  { return o1.totalBytes() > o2.totalBytes();  }

}
}
}

#endif // FILESIZE_H
