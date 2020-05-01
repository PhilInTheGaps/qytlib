#ifndef STREAMINFOPROVIDER_H
#define STREAMINFOPROVIDER_H

#include "iresponse.h"

namespace YouTube {
namespace Responses {

class IStreamInfoProvider : public IResponse
{
    Q_OBJECT

public:
    IStreamInfoProvider(QObject *parent = nullptr) : IResponse(parent) {}
    virtual ~IStreamInfoProvider() {};

    virtual int tag() const = 0;

    virtual QString url() const = 0;

    virtual QString signature() const = 0;

    virtual QString signatureParameter() const = 0;

    virtual qint64 contentLength() const = 0;

    virtual qint64 bitrate() const = 0;

    virtual QString mimeType() const = 0;

    virtual QString container() const = 0;

    virtual bool isAudioOnly() const = 0;

    virtual QString codecs() const = 0;

    virtual QString audioCodec() const = 0;

    virtual QString videoCodec() const = 0;

    virtual QString videoQualityLabel() const = 0;

    virtual int videoWidth() const = 0;

    virtual int videoHeight() const = 0;

    virtual int framerate() const = 0;
};

}
}

#endif // STREAMINFOPROVIDER_H
