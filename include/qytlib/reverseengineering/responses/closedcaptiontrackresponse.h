#ifndef CLOSEDCAPTIONTRACKRESPONSE_H
#define CLOSEDCAPTIONTRACKRESPONSE_H

#include <QNetworkAccessManager>
#include <QLoggingCategory>

#include "iresponse.h"
#include "qytlib/videos/closedcaptions/closedcaption.h"

Q_DECLARE_LOGGING_CATEGORY(ytClosedCaptionTrackResponse)

namespace YouTube {
namespace Responses {

class ClosedCaptionTrackResponse : public IResponse
{
    Q_OBJECT
public:
    explicit ClosedCaptionTrackResponse(QObject *parent = nullptr) : IResponse(parent) {}

    /// Get ClosedCaptionTrackResponse from URL.
    static ClosedCaptionTrackResponse *get(QNetworkAccessManager *networkManager, const QString &url, QObject *parent);

    /// Load the Response by parsing raw XML data.
    void parse(const QByteArray &raw);

    QList<Videos::CC::ClosedCaption*> closedCaptions() const { return m_closedCaptions; }

private:
    QList<Videos::CC::ClosedCaption*> m_closedCaptions;
};

}
}

#endif // CLOSEDCAPTIONTRACKRESPONSE_H
