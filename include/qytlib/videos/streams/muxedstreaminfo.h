#ifndef MUXEDSTREAMINFO_H
#define MUXEDSTREAMINFO_H

#include "istreaminfo.h"
#include "iaudiostreaminfo.h"
#include "ivideostreaminfo.h"

namespace YouTube {
namespace Videos {
namespace Streams {

class MuxedStreamInfo : public IStreamInfo, public IAudioStreamInfo, public IVideoStreamInfo
{
    Q_OBJECT

public:
    explicit MuxedStreamInfo(const int& tag, const QString& url, Container *container,
                             FileSize *fileSize, Bitrate *bitrate, const QString& audioCodec,
                             const QString& videoCodec, const QString& videoQualityLabel,
                             const VideoQuality& videoQuality, const VideoResolution& videoResolution,
                             const int& frameRate, QObject *parent = nullptr)
        : IStreamInfo(tag, url, container, fileSize, bitrate, parent), IAudioStreamInfo(audioCodec),
          IVideoStreamInfo(videoCodec, videoQualityLabel, videoQuality, videoResolution, frameRate) {}

    inline operator const QString() const { return QString("Muxed (%1 | %2 | %3)").arg(QString::number(tag()), videoQualityLabel(), *container()); }

    /// Get the MuxedStreamInfo* with the highest bitrate
    static MuxedStreamInfo* withHighestBitrate(QVector<MuxedStreamInfo*> vector) {
        std::sort(vector.begin(), vector.end(), &MuxedStreamInfo::compareBitrateGreater);
        return vector.isEmpty() ? nullptr : vector.first();
    }
};

}
}
}

#endif // MUXEDSTREAMINFO_H
