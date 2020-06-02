#ifndef VIDEOONLYSTREAMINFO_H
#define VIDEOONLYSTREAMINFO_H

#include "istreaminfo.h"
#include "ivideostreaminfo.h"

namespace YouTube {
namespace Videos {
namespace Streams {

class VideoOnlyStreamInfo : public IStreamInfo, public IVideoStreamInfo
{
    Q_OBJECT

public:
    explicit VideoOnlyStreamInfo(const int& tag, const QString& url, Container *container,
                                 FileSize *fileSize, Bitrate *bitrate, const QString& videoCodec, const QString& videoQualityLabel,
                                 const VideoQuality& videoQuality, const VideoResolution& videoResolution,
                                 const int& frameRate, QObject *parent = nullptr)
        : IStreamInfo(tag, url, container, fileSize, bitrate, parent),
          IVideoStreamInfo(videoCodec, videoQualityLabel, videoQuality, videoResolution, frameRate) {}

    inline operator const QString() const { return QString("Video-only (%1 | %2 | %3)").arg(QString::number(tag()), videoQualityLabel(), *container()); }
};

}
}
}

#endif // VIDEOONLYSTREAMINFO_H
