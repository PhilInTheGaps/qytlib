#ifndef IVIDEOSTREAMINFO_H
#define IVIDEOSTREAMINFO_H

#include <QString>
#include "reverseengineering/heuristics.h"

namespace YouTube {
namespace Videos {
namespace Streams {

class IVideoStreamInfo
{
public:
    explicit IVideoStreamInfo(const QString& videoCodec, const QString& videoQualityLabel,
                              const VideoQuality& videoQuality, const VideoResolution& videoResolution,
                              const int& frameRate)
        : m_videoCodec(videoCodec), m_videoQualityLabel(videoQualityLabel),
          m_videoQuality(videoQuality), m_videoResolution(videoResolution), m_frameRate(frameRate) {}

    QString videoCodec() const { return m_videoCodec; }

    QString videoQualityLabel() const { return m_videoQualityLabel; }

    int frameRate() const { return m_frameRate; }

    VideoQuality videoQuality() const { return m_videoQuality; }

    VideoResolution videoResolution() const { return m_videoResolution; }

protected:
    QString m_videoCodec;
    QString m_videoQualityLabel;

    VideoQuality m_videoQuality = VideoQuality::Low144;
    VideoResolution m_videoResolution;

    int m_frameRate = 0;
};

}
}
}

#endif // IVIDEOSTREAMINFO_H
