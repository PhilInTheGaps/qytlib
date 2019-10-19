#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <QObject>
#include "models/mediastreams/mediastreaminfo.h"
#include "models/mediastreams/videoresolution.h"

class Heuristics
{
public:
    Heuristics();

    static MediaStreamInfo::Container containerFromString(QString str);

    static MediaStreamInfo::AudioEncoding audioEncodingFromString(QString str);

    static QString audioEncodingToString(MediaStreamInfo::AudioEncoding encoding);

    static MediaStreamInfo::VideoEncoding videoEncodingFromString(QString str);

    static QString containerToFileExtension(MediaStreamInfo::Container container);

    static MediaStreamInfo::VideoQuality videoQualityFromItag(int itag);

    static MediaStreamInfo::VideoQuality videoQualityFromLabel(QString label);

    static QString videoQualityToLabel(MediaStreamInfo::VideoQuality quality);

    static QString videoQualityToLabel(MediaStreamInfo::VideoQuality quality, int framerate);

    static VideoResolution videoQualityToResolution(MediaStreamInfo::VideoQuality quality);

private:
    static QMap<MediaStreamInfo::Container, QString> containerToFileExtensionMap();

    static QMap<int, MediaStreamInfo::VideoQuality> itagToVideoQualityMap();

    static QMap<MediaStreamInfo::VideoQuality, VideoResolution> videoQualityToResolutionMap();

};

#endif // HEURISTICS_H
