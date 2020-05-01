#ifndef AUDIOONLYSTREAMINFO_H
#define AUDIOONLYSTREAMINFO_H

#include "istreaminfo.h"
#include "iaudiostreaminfo.h"

namespace YouTube {
namespace Videos {
namespace Streams {

class AudioOnlyStreamInfo : public IStreamInfo, public IAudioStreamInfo
{
    Q_OBJECT

public:
    explicit AudioOnlyStreamInfo(const int& tag, const QString& url, Container *container,
                                 FileSize *fileSize, Bitrate *bitrate, const QString& audioCodec, QObject *parent = nullptr)
            : IStreamInfo(tag, url, container, fileSize, bitrate, parent), IAudioStreamInfo(audioCodec) {};

    inline operator const QString() const { return QString("Audio-only (%1 | %2)").arg(tag()).arg(*container()); }
};

}
}
}

#endif // AUDIOONLYSTREAMINFO_H
