#include "heuristics.h"
#include "youtubeutils.h"
#include <QtMath>

Heuristics::Heuristics()
{
}

MediaStreamInfo::Container Heuristics::containerFromString(QString str)
{
    if (str.toLower() == "mp4") return MediaStreamInfo::Mp4;

    if (str.toLower() == "webm") return MediaStreamInfo::WebM;

    if (str.toLower() == "3gpp") return MediaStreamInfo::Tgpp;

    throw std::invalid_argument(QString("Unrecognized container [%1].").arg(str).toUtf8());
}

MediaStreamInfo::AudioEncoding Heuristics::audioEncodingFromString(QString str)
{
    if (str.toLower().startsWith("mp4a")) return MediaStreamInfo::Aac;

    if (str.toLower().startsWith("vorbis")) return MediaStreamInfo::Vorbis;

    if (str.toLower().startsWith("opus")) return MediaStreamInfo::Opus;

    throw std::invalid_argument(QString("Unrecognized audio encoding [%1].").arg(str).toUtf8());
}

QString Heuristics::audioEncodingToString(MediaStreamInfo::AudioEncoding encoding)
{
    switch (encoding)
    {
    case MediaStreamInfo::Aac: return "mp4";

    case MediaStreamInfo::Vorbis: return "vorbis";

    case MediaStreamInfo::Opus: return "opus";

    default:
        throw std::invalid_argument(QString("Unrecognized audio encoding [%1].").arg(encoding).toUtf8());
    }
}

MediaStreamInfo::VideoEncoding Heuristics::videoEncodingFromString(QString str)
{
    if (str.toLower().startsWith("mp4v")) return MediaStreamInfo::Mp4V;

    if (str.toLower().startsWith("avc1")) return MediaStreamInfo::H264;

    if (str.toLower().startsWith("vp8")) return MediaStreamInfo::Vp8;

    if (str.toLower().startsWith("vp9")) return MediaStreamInfo::Vp9;

    if (str.toLower().startsWith("av01")) return MediaStreamInfo::Av1;

    throw std::invalid_argument(QString("Unrecognized video encoding [%1].").arg(str).toUtf8());
}

QString Heuristics::containerToFileExtension(MediaStreamInfo::Container container)
{
    if (containerToFileExtensionMap().contains(container))
    {
        return containerToFileExtensionMap()[container];
    }

    throw std::invalid_argument(QString("Unrecognized container [%1].").arg(container).toUtf8());
}

MediaStreamInfo::VideoQuality Heuristics::videoQualityFromItag(int itag)
{
    if (itagToVideoQualityMap().contains(itag))
    {
        return itagToVideoQualityMap()[itag];
    }

    throw std::invalid_argument(QString("Unrecognized itag [%1].").arg(itag).toUtf8());
}

MediaStreamInfo::VideoQuality Heuristics::videoQualityFromLabel(QString label)
{
    if (label.startsWith("144p")) return MediaStreamInfo::Low144;

    if (label.startsWith("240p")) return MediaStreamInfo::Low240;

    if (label.startsWith("360p")) return MediaStreamInfo::Medium360;

    if (label.startsWith("480p")) return MediaStreamInfo::Medium480;

    if (label.startsWith("720p")) return MediaStreamInfo::High720;

    if (label.startsWith("1080p")) return MediaStreamInfo::High1080;

    if (label.startsWith("1440p")) return MediaStreamInfo::High1440;

    if (label.startsWith("2160p")) return MediaStreamInfo::High2160;

    if (label.startsWith("2880p")) return MediaStreamInfo::High2880;

    if (label.startsWith("3072p")) return MediaStreamInfo::High3072;

    if (label.startsWith("4320p")) return MediaStreamInfo::High4320;

    // Unrecognized
    throw std::invalid_argument(QString("Unrecognized video quality label [%1].").arg(label).toUtf8());
}

QString Heuristics::videoQualityToLabel(MediaStreamInfo::VideoQuality quality)
{
    // Convert to string, strip non-digits and add "p"
    QMetaEnum metaEnum = QMetaEnum::fromType<MediaStreamInfo::VideoQuality>();

    return YouTubeUtils::regexMatch(metaEnum.valueToKey(quality), "/\\D/g") + "p";
}

QString Heuristics::videoQualityToLabel(MediaStreamInfo::VideoQuality quality, int framerate)
{
    // Framerate appears only if it's above 30
    if (framerate <= 30) return videoQualityToLabel(quality);

    // YouTube rounds framerate to nearest next decimal
    auto framerateRounded = qCeil(framerate / 10.0) * 10;
    return videoQualityToLabel(quality) + QString::number(framerateRounded);
}

VideoResolution Heuristics::videoQualityToResolution(MediaStreamInfo::VideoQuality quality)
{
    if (videoQualityToResolutionMap().contains(quality))
    {
        return videoQualityToResolutionMap().value(quality);
    }

    throw std::invalid_argument(QString("Unrecognized video quality [%1].").arg(quality).toUtf8());
}

QMap<MediaStreamInfo::Container, QString>Heuristics::containerToFileExtensionMap()
{
    return QMap<MediaStreamInfo::Container, QString>({
        { MediaStreamInfo::Mp4, "mp4" },
        { MediaStreamInfo::WebM, "webm" },
        { MediaStreamInfo::Tgpp, "3gpp" }
    });
}

QMap<int, MediaStreamInfo::VideoQuality>Heuristics::itagToVideoQualityMap()
{
    return QMap<int, MediaStreamInfo::VideoQuality>({
        { 5, MediaStreamInfo::Low144 },
        { 6, MediaStreamInfo::Low240 },
        { 13, MediaStreamInfo::Low144 },
        { 17, MediaStreamInfo::Low144 },
        { 18, MediaStreamInfo::Medium360 },
        { 22, MediaStreamInfo::High720 },
        { 34, MediaStreamInfo::Medium360 },
        { 35, MediaStreamInfo::Medium480 },
        { 36, MediaStreamInfo::Low240 },
        { 37, MediaStreamInfo::High1080 },
        { 38, MediaStreamInfo::High3072 },
        { 43, MediaStreamInfo::Medium360 },
        { 44, MediaStreamInfo::Medium480 },
        { 45, MediaStreamInfo::High720 },
        { 46, MediaStreamInfo::High1080 },
        { 59, MediaStreamInfo::Medium480 },
        { 78, MediaStreamInfo::Medium480 },
        { 82, MediaStreamInfo::Medium360 },
        { 83, MediaStreamInfo::Medium480 },
        { 84, MediaStreamInfo::High720 },
        { 85, MediaStreamInfo::High1080 },
        { 91, MediaStreamInfo::Low144 },
        { 92, MediaStreamInfo::Low240 },
        { 93, MediaStreamInfo::Medium360 },
        { 94, MediaStreamInfo::Medium480 },
        { 95, MediaStreamInfo::High720 },
        { 96, MediaStreamInfo::High1080 },
        { 100, MediaStreamInfo::Medium360 },
        { 101, MediaStreamInfo::Medium480 },
        { 102, MediaStreamInfo::High720 },
        { 132, MediaStreamInfo::Low240 },
        { 151, MediaStreamInfo::Low144 },
        { 133, MediaStreamInfo::Low240 },
        { 134, MediaStreamInfo::Medium360 },
        { 135, MediaStreamInfo::Medium480 },
        { 136, MediaStreamInfo::High720 },
        { 137, MediaStreamInfo::High1080 },
        { 138, MediaStreamInfo::High4320 },
        { 160, MediaStreamInfo::Low144 },
        { 212, MediaStreamInfo::Medium480 },
        { 213, MediaStreamInfo::Medium480 },
        { 214, MediaStreamInfo::High720 },
        { 215, MediaStreamInfo::High720 },
        { 216, MediaStreamInfo::High1080 },
        { 217, MediaStreamInfo::High1080 },
        { 264, MediaStreamInfo::High1440 },
        { 266, MediaStreamInfo::High2160 },
        { 298, MediaStreamInfo::High720 },
        { 299, MediaStreamInfo::High1080 },
        { 399, MediaStreamInfo::High1080 },
        { 398, MediaStreamInfo::High720 },
        { 397, MediaStreamInfo::Medium480 },
        { 396, MediaStreamInfo::Medium360 },
        { 395, MediaStreamInfo::Low240 },
        { 394, MediaStreamInfo::Low144 },
        { 167, MediaStreamInfo::Medium360 },
        { 168, MediaStreamInfo::Medium480 },
        { 169, MediaStreamInfo::High720 },
        { 170, MediaStreamInfo::High1080 },
        { 218, MediaStreamInfo::Medium480 },
        { 219, MediaStreamInfo::Medium480 },
        { 242, MediaStreamInfo::Low240 },
        { 243, MediaStreamInfo::Medium360 },
        { 244, MediaStreamInfo::Medium480 },
        { 245, MediaStreamInfo::Medium480 },
        { 246, MediaStreamInfo::Medium480 },
        { 247, MediaStreamInfo::High720 },
        { 248, MediaStreamInfo::High1080 },
        { 271, MediaStreamInfo::High1440 },
        { 272, MediaStreamInfo::High2160 },
        { 278, MediaStreamInfo::Low144 },
        { 302, MediaStreamInfo::High720 },
        { 303, MediaStreamInfo::High1080 },
        { 308, MediaStreamInfo::High1440 },
        { 313, MediaStreamInfo::High2160 },
        { 315, MediaStreamInfo::High2160 },
        { 330, MediaStreamInfo::Low144 },
        { 331, MediaStreamInfo::Low240 },
        { 332, MediaStreamInfo::Medium360 },
        { 333, MediaStreamInfo::Medium480 },
        { 334, MediaStreamInfo::High720 },
        { 335, MediaStreamInfo::High1080 },
        { 336, MediaStreamInfo::High1440 },
        { 337, MediaStreamInfo::High2160 }
    });
}

QMap<MediaStreamInfo::VideoQuality, VideoResolution>Heuristics::videoQualityToResolutionMap()
{
    return QMap<MediaStreamInfo::VideoQuality, VideoResolution>({
        { MediaStreamInfo::Low144, VideoResolution(256, 144) },
        { MediaStreamInfo::Low240, VideoResolution(426, 240) },
        { MediaStreamInfo::Medium360, VideoResolution(640, 360) },
        { MediaStreamInfo::Medium480, VideoResolution(854, 480) },
        { MediaStreamInfo::High720,  VideoResolution(1280, 720) },
        { MediaStreamInfo::High1080,  VideoResolution(1920, 1080) },
        { MediaStreamInfo::High1440,  VideoResolution(2560, 1440) },
        { MediaStreamInfo::High2160,  VideoResolution(3840, 2160) },
        { MediaStreamInfo::High2880,  VideoResolution(5120, 2880) },
        { MediaStreamInfo::High3072,  VideoResolution(4096, 3072) },
        { MediaStreamInfo::High4320,  VideoResolution(7680, 4320) }
    });
}
