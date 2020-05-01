#ifndef STREAMMANIFEST_H
#define STREAMMANIFEST_H

#include "videoonlystreaminfo.h"
#include "audioonlystreaminfo.h"
#include "muxedstreaminfo.h"
#include "reverseengineering/responses/iresponse.h"

namespace YouTube {
namespace Videos {
namespace Streams {

class StreamManifest : public Responses::IResponse
{
    Q_OBJECT

public:
    explicit StreamManifest(QObject *parent = nullptr) : Responses::IResponse(parent) {}
    explicit StreamManifest(const QList<IStreamInfo*>& streams, QObject *parent = nullptr);

    void parse(const QList<IStreamInfo *> &streams);
    void setReady() { emit ready(); }

    /// Available streams.
    QList<IStreamInfo *> streams() const { return m_streams; }

    /// Gets audio-only streams (no video).
    QVector<AudioOnlyStreamInfo *> audio() const;

    /// Gets video-only streams (no audio).
    /// These streams have the widest range of qualities available.
    QVector<VideoOnlyStreamInfo *> video() const;

    /// Gets muxed streams (contain both audio and video).
    /// Note that muxed streams are limited in quality and don't go beyond 720p30.
    QVector<MuxedStreamInfo *> muxed() const;

private:
    QList<IStreamInfo*> m_streams;
};

}
}
}



#endif // STREAMMANIFEST_H
