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

    /// Get the AudioOnlyStreamInfo* with the highest bitrate
    static AudioOnlyStreamInfo* withHighestBitrate(QVector<AudioOnlyStreamInfo*> vector) {
        std::sort(vector.begin(), vector.end(), &AudioOnlyStreamInfo::compareBitrateGreater);
        return vector.isEmpty() ? nullptr : vector.first();
    }
};

}
}
}

#endif // AUDIOONLYSTREAMINFO_H
