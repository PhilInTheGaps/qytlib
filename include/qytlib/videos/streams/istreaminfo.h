#ifndef ISTREAMINFO_H
#define ISTREAMINFO_H

#include <QString>
#include <algorithm>

#include "container.h"
#include "filesize.h"
#include "bitrate.h"
#include "qytlib/utils/regexutils.h"

namespace YouTube {
namespace Videos {
namespace Streams {

class IStreamInfo : public QObject
{
    Q_OBJECT

public:
    explicit IStreamInfo(const int& tag, const QString& url, Container *container,
                         FileSize *fileSize, Bitrate *bitrate, QObject *parent = nullptr)
        : QObject(parent), m_tag(tag), m_url(url), m_container(container), m_fileSize(fileSize), m_bitrate(bitrate)
    {
        m_container->setParent(this);
        m_fileSize->setParent(this);
        m_bitrate->setParent(this);
    }
    virtual ~IStreamInfo() {}

    /// Stream tag.
    /// Uniquely identifies a stream inside a manifest.
    int tag() const { return m_tag; }

    /// Stream URL.
    QString url() const { return m_url; }

    /// Stream container.
    Container* container() const { return m_container; }

    /// Stream size.
    FileSize* fileSize() const { return m_fileSize; }

    /// Stream bitrate.
    Bitrate* bitrate() const { return m_bitrate; }

    /// Is the download rate limited?
    bool isRateLimited() const { return !Utils::RegExUtils::isMatch(url(), "ratebypass[=/]yes"); }

    /// Returns true if bitrate of o1 is greater than bitrate of o2
    static bool compareBitrateGreater(IStreamInfo *o1, IStreamInfo *o2) { return o1->bitrate() > o2->bitrate(); }

    /// Get the IStreamInfo* with the highest bitrate
    static IStreamInfo* withHighestBitrate(QVector<IStreamInfo*> vector) {
        std::sort(vector.begin(), vector.end(), &IStreamInfo::compareBitrateGreater);
        return vector.isEmpty() ? nullptr : vector.first();
    }

protected:
    int m_tag = 0;
    QString m_url;

    Container* m_container = nullptr;
    FileSize*  m_fileSize = nullptr;
    Bitrate*   m_bitrate = nullptr;
};

}
}
}

#endif // ISTREAMINFO_H
