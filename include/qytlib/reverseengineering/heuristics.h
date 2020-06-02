#ifndef HEURISTICS_H
#define HEURISTICS_H

#include <QMap>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(ytHeuristics)

namespace YouTube {

enum VideoQuality {
    Low144,
    Low240,
    Medium360,
    Medium480,
    High720,
    High1080,
    High1440,
    High2160,
    High2880,
    High3072,
    High4320
};

class VideoResolution {
public:
    VideoResolution() {}
    VideoResolution(const int& width, const int& height) : m_width(width), m_height(height) {}

    inline operator QString() const { return QString("%1x%2").arg(m_width, m_height) ; }

    int height() const { return m_height; }
    int width()  const { return m_width;  }

private:
    int m_width = 0;
    int m_height = 0;
};

inline bool operator==(const VideoResolution& o1, const VideoResolution& o2) { return o1.width() == o2.width() && o1.height() == o2.height(); }
inline bool operator!=(const VideoResolution& o1, const VideoResolution& o2) { return !(o1 == o2); }

class Heuristics
{
public:
    static VideoQuality videoQuality(const int& tag);

    static VideoQuality videoQuality(const QString& label);

    static QString videoQualityLabel(const VideoQuality& quality);

    static QString videoQualityLabel(const VideoQuality& quality, const qreal& framerate);

    static QString videoQualityLabel(const int& tag, const qreal& framerate);

    static VideoResolution videoResolution(const VideoQuality& quality);

private:
    static QMap<int, VideoQuality> tagToVideoQualityMap;

    static QMap<VideoQuality, VideoResolution> videoQualityToResolutionMap;
};

}

#endif // HEURISTICS_H
