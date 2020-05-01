#include "streammanifest.h"

using namespace YouTube::Videos::Streams;

StreamManifest::StreamManifest(const QList<IStreamInfo *> &streams, QObject *parent)
    : Responses::IResponse(parent)
{
    parse(streams);
}

void StreamManifest::parse(const QList<IStreamInfo *> &streams)
{
    m_streams = streams;

    for (const auto& stream : streams)
        stream->setParent(this);
}

/// Gets streams that contain audio (which includes muxed and audio-only streams).
QVector<AudioOnlyStreamInfo *> StreamManifest::audio() const
{
    QVector<AudioOnlyStreamInfo*> vector;

    for (const auto& stream : m_streams)
    {
        if (AudioOnlyStreamInfo* info = dynamic_cast<AudioOnlyStreamInfo*>(stream))
        {
            vector.append(info);
        }
    }

    return vector;
}

/// Gets streams that contain video (which includes muxed and video-only streams).
QVector<VideoOnlyStreamInfo *> StreamManifest::video() const
{
    QVector<VideoOnlyStreamInfo*> vector;

    for (const auto& stream : m_streams)
    {
        if (VideoOnlyStreamInfo* info = dynamic_cast<VideoOnlyStreamInfo*>(stream))
        {
            vector.append(info);
        }
    }

    return vector;
}

/// Gets muxed streams (contain both audio and video).
/// Note that muxed streams are limited in quality and don't go beyond 720p30.
QVector<MuxedStreamInfo *> StreamManifest::muxed() const
{
    QVector<MuxedStreamInfo*> vector;

    for (const auto& stream : m_streams)
    {
        if (MuxedStreamInfo* info = dynamic_cast<MuxedStreamInfo*>(stream))
        {
            vector.append(info);
        }
    }

    return vector;
}
