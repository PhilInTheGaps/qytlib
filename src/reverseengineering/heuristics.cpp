#include "heuristics.h"
#include <QtMath>

Q_LOGGING_CATEGORY(ytHeuristics, "yt.heuristics")

using namespace YouTube;

QMap<int, VideoQuality> Heuristics::tagToVideoQualityMap = {
    {   5, VideoQuality::Low144    },
    {   6, VideoQuality::Low240    },
    {  13, VideoQuality::Low144    },
    {  17, VideoQuality::Low144    },
    {  18, VideoQuality::Medium360 },
    {  22, VideoQuality::High720   },
    {  34, VideoQuality::Medium360 },
    {  35, VideoQuality::Medium480 },
    {  36, VideoQuality::Low240    },
    {  37, VideoQuality::High1080  },
    {  38, VideoQuality::High3072  },
    {  43, VideoQuality::Medium360 },
    {  44, VideoQuality::Medium480 },
    {  45, VideoQuality::High720   },
    {  46, VideoQuality::High1080  },
    {  59, VideoQuality::Medium480 },
    {  78, VideoQuality::Medium480 },
    {  82, VideoQuality::Medium360 },
    {  83, VideoQuality::Medium480 },
    {  84, VideoQuality::High720   },
    {  85, VideoQuality::High1080  },
    {  91, VideoQuality::Low144    },
    {  92, VideoQuality::Low240    },
    {  93, VideoQuality::Medium360 },
    {  94, VideoQuality::Medium480 },
    {  95, VideoQuality::High720   },
    {  96, VideoQuality::High1080  },
    { 100, VideoQuality::Medium360 },
    { 101, VideoQuality::Medium480 },
    { 102, VideoQuality::High720   },
    { 132, VideoQuality::Low240    },
    { 151, VideoQuality::Low144    },
    { 133, VideoQuality::Low240    },
    { 134, VideoQuality::Medium360 },
    { 135, VideoQuality::Medium480 },
    { 136, VideoQuality::High720   },
    { 137, VideoQuality::High1080  },
    { 138, VideoQuality::High4320  },
    { 160, VideoQuality::Low144    },
    { 212, VideoQuality::Medium480 },
    { 213, VideoQuality::Medium480 },
    { 214, VideoQuality::High720   },
    { 215, VideoQuality::High720   },
    { 216, VideoQuality::High1080  },
    { 217, VideoQuality::High1080  },
    { 264, VideoQuality::High1440  },
    { 266, VideoQuality::High2160  },
    { 298, VideoQuality::High720   },
    { 299, VideoQuality::High1080  },
    { 399, VideoQuality::High1080  },
    { 398, VideoQuality::High720   },
    { 397, VideoQuality::Medium480 },
    { 396, VideoQuality::Medium360 },
    { 395, VideoQuality::Low240    },
    { 394, VideoQuality::Low144    },
    { 167, VideoQuality::Medium360 },
    { 168, VideoQuality::Medium480 },
    { 169, VideoQuality::High720   },
    { 170, VideoQuality::High1080  },
    { 218, VideoQuality::Medium480 },
    { 219, VideoQuality::Medium480 },
    { 242, VideoQuality::Low240    },
    { 243, VideoQuality::Medium360 },
    { 244, VideoQuality::Medium480 },
    { 245, VideoQuality::Medium480 },
    { 246, VideoQuality::Medium480 },
    { 247, VideoQuality::High720   },
    { 248, VideoQuality::High1080  },
    { 271, VideoQuality::High1440  },
    { 272, VideoQuality::High2160  },
    { 278, VideoQuality::Low144    },
    { 302, VideoQuality::High720   },
    { 303, VideoQuality::High1080  },
    { 308, VideoQuality::High1440  },
    { 313, VideoQuality::High2160  },
    { 315, VideoQuality::High2160  },
    { 330, VideoQuality::Low144    },
    { 331, VideoQuality::Low240    },
    { 332, VideoQuality::Medium360 },
    { 333, VideoQuality::Medium480 },
    { 334, VideoQuality::High720   },
    { 335, VideoQuality::High1080  },
    { 336, VideoQuality::High1440  },
    { 337, VideoQuality::High2160  }
};

QMap<VideoQuality, VideoResolution> Heuristics::videoQualityToResolutionMap = {
    { VideoQuality::Low144,    VideoResolution( 256,  144) },
    { VideoQuality::Low240,    VideoResolution( 426,  240) },
    { VideoQuality::Medium360, VideoResolution( 640,  360) },
    { VideoQuality::Medium480, VideoResolution( 854,  480) },
    { VideoQuality::High720,   VideoResolution(1280,  720) },
    { VideoQuality::High1080,  VideoResolution(1920, 1080) },
    { VideoQuality::High1440,  VideoResolution(2560, 1440) },
    { VideoQuality::High2160,  VideoResolution(3840, 2160) },
    { VideoQuality::High2880,  VideoResolution(5120, 2880) },
    { VideoQuality::High3072,  VideoResolution(4096, 3072) },
    { VideoQuality::High4320,  VideoResolution(7680, 4320) }
};

VideoQuality Heuristics::videoQuality(const int &tag)
{
    if (Heuristics::tagToVideoQualityMap.contains(tag))
        return Heuristics::tagToVideoQualityMap[tag];

    qCWarning(ytHeuristics()) << "Warning: Unrecognized tag" << tag << "defaulting to 144p";

    return VideoQuality::Low144;
}

VideoQuality Heuristics::videoQuality(const QString &label)
{
    if (label.startsWith("144p", Qt::CaseInsensitive))
         return VideoQuality::Low144;

    if (label.startsWith("240p", Qt::CaseInsensitive))
        return VideoQuality::Low240;

    if (label.startsWith("360p", Qt::CaseInsensitive))
        return VideoQuality::Medium360;

    if (label.startsWith("480p", Qt::CaseInsensitive))
        return VideoQuality::Medium480;

    if (label.startsWith("720p", Qt::CaseInsensitive))
        return VideoQuality::High720;

    if (label.startsWith("1080p", Qt::CaseInsensitive))
        return VideoQuality::High1080;

    if (label.startsWith("1440p", Qt::CaseInsensitive))
        return VideoQuality::High1440;

    if (label.startsWith("2160p", Qt::CaseInsensitive))
        return VideoQuality::High2160;

    if (label.startsWith("2880p", Qt::CaseInsensitive))
        return VideoQuality::High2880;

    if (label.startsWith("3072p", Qt::CaseInsensitive))
        return VideoQuality::High3072;

    if (label.startsWith("4320p", Qt::CaseInsensitive))
        return VideoQuality::High4320;

    qCWarning(ytHeuristics()) << "Warning:" << label << "is an unrecognized video quality, defaulting to 144p";

    return VideoQuality::Low144;
}

QString Heuristics::videoQualityLabel(const VideoQuality &quality)
{
    switch (quality)
    {
    case VideoQuality::Low144:    return  "144p";
    case VideoQuality::Low240:    return  "240p";
    case VideoQuality::Medium360: return  "360p";
    case VideoQuality::Medium480: return  "480p";
    case VideoQuality::High720:   return  "720p";
    case VideoQuality::High1080:  return "1080p";
    case VideoQuality::High1440:  return "1440p";
    case VideoQuality::High2160:  return "2160p";
    case VideoQuality::High2880:  return "2880p";
    case VideoQuality::High3072:  return "3072p";
    case VideoQuality::High4320:  return "4320p";
    default: return "unknown";
    }
}

QString Heuristics::videoQualityLabel(const VideoQuality &quality, const qreal &framerate)
{
    auto label = videoQualityLabel(quality);

    // Framerate appears only if it's above 30
    if (framerate <= 30) return label;

    // YouTube rounds framerate to nearest next decimal
    auto rounded = qCeil(framerate / 10.0) * 10;

    return label + QString::number(rounded);
}

QString Heuristics::videoQualityLabel(const int &tag, const qreal &framerate)
{
    auto videoQuality = Heuristics::videoQuality(tag);
    return videoQualityLabel(videoQuality, framerate);
}

VideoResolution Heuristics::videoResolution(const VideoQuality &quality)
{
    if (videoQualityToResolutionMap.contains(quality))
        return videoQualityToResolutionMap.value(quality);

    qCWarning(ytHeuristics()) << "Warning: Unrecognized quality" << quality << "defaulting to 144p";
    return videoQualityToResolutionMap[VideoQuality::Low144];
}
