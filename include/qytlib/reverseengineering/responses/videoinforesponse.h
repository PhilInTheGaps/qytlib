#ifndef VIDEOINFORESPONSE_H
#define VIDEOINFORESPONSE_H

#include <QLoggingCategory>
#include <QUrlQuery>
#include <QNetworkAccessManager>

#include "iresponse.h"
#include "playerresponse.h"
#include "istreaminfoprovider.h"
#include "qytlib/utils/stringutils.h"
#include "qytlib/videos/videoid.h"

Q_DECLARE_LOGGING_CATEGORY(ytVideoInfoResponse)

namespace YouTube {
namespace Responses {

class VideoInfoResponse : public IResponse
{
    Q_OBJECT

public:
    explicit VideoInfoResponse(QObject *parent = nullptr) : IResponse(parent) {}

    static VideoInfoResponse* get(QNetworkAccessManager *networkManager, const Videos::VideoId& videoId, const QString& sts = "", QObject *parent = nullptr);

    void parse(const QByteArray& raw);

    QString status() const { return m_root.queryItemValue("status"); }

    bool isVideoAvailable() const { return QString::compare(status(), "fail", Qt::CaseInsensitive); }

    PlayerResponse* playerResponse() const { return m_playerResponse; }

    QVector<IStreamInfoProvider*> muxedStreams() const { return m_muxedStreams; }

    QVector<IStreamInfoProvider*> adaptiveStreams() const { return m_adaptiveStreams; }

    QVector<IStreamInfoProvider*> streams() const { return m_muxedStreams + m_adaptiveStreams; }

private:
    QUrlQuery m_root;
    PlayerResponse* m_playerResponse = nullptr;

    QVector<IStreamInfoProvider*> m_muxedStreams;
    QVector<IStreamInfoProvider*> m_adaptiveStreams;

    void loadMuxedStreams();
    void loadAdaptiveStreams();
};

}
}

#endif // VIDEOINFORESPONSE_H
