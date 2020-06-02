#ifndef STREAMINFOVIR_H
#define STREAMINFOVIR_H

#include <QUrlQuery>
#include "istreaminfoprovider.h"
#include "qytlib/utils/stringutils.h"

namespace YouTube {
namespace Responses {

class StreamInfoVIR : public IStreamInfoProvider
{
    Q_OBJECT

public:
    StreamInfoVIR(const QUrlQuery& root, QObject *parent = nullptr) : IStreamInfoProvider(parent), m_root(root) {}
    ~StreamInfoVIR() override {}

    int tag() const override { return m_root.queryItemValue("itag").toInt(); }

    QString url() const override { return m_root.queryItemValue("url"); }

    QString signature() const override { return m_root.queryItemValue("s"); }

    QString signatureParameter() const override { return m_root.queryItemValue("sp"); }

    qint64 contentLength() const override;

    qint64 bitrate() const override { return m_root.queryItemValue("bitrate").toLong(); }

    QString mimeType() const override { return m_root.queryItemValue("type"); }

    QString container() const override;

    bool isAudioOnly() const override { return mimeType().startsWith("audio/", Qt::CaseInsensitive); }

    QString codecs() const override;

    QString audioCodec() const override { return isAudioOnly() ? codecs() : Utils::StringUtils::substringAfter(codecs(), ", "); }

    QString videoCodec() const override { return isAudioOnly() ? "" : Utils::StringUtils::substringUntil(codecs(), ", "); }

    QString videoQualityLabel() const override { return m_root.queryItemValue("quality_label"); }

    int videoWidth() const override;

    int videoHeight() const override;

    int framerate() const override { return m_root.queryItemValue("fps").toInt(); }

private:
    QUrlQuery m_root;
};

}
}

#endif // STREAMINFOVIR_H
