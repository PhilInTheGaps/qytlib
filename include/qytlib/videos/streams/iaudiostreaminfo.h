#ifndef IAUDIOSTREAMINFO_H
#define IAUDIOSTREAMINFO_H

#include <QString>

namespace YouTube {
namespace Videos {
namespace Streams {

class IAudioStreamInfo
{

public:
    explicit IAudioStreamInfo(const QString& audioCodec) : m_audioCodec(audioCodec) {}

    QString audioCodec() const { return m_audioCodec; }

protected:
    QString m_audioCodec;
};

}
}
}

#endif // IAUDIOSTREAMINFO_H
