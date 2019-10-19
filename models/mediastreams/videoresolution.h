#ifndef VIDEORESOLUTION_H
#define VIDEORESOLUTION_H

#include <QObject>

class VideoResolution
{
public:
    VideoResolution(int width, int height) : m_width(width), m_height(height) {}

    VideoResolution() {}

    /**
     * @brief Viewport width.
     */
    int width() const { return m_width; }

    /**
     * @brief Viewport height.
     */
    int height() const { return m_height; }

    bool equals(const VideoResolution& other) const { return (m_width == other.m_width && m_height == other.m_height); }

    int getHashCode() const { return (m_width * 397) ^ m_height; }

    QString toString() const { return QString("%1x%2").arg(m_width).arg(m_height); }

private:
    int m_width, m_height;

};

#endif // VIDEORESOLUTION_H
