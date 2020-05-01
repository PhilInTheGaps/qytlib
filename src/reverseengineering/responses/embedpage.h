#ifndef EMBEDPAGE_H
#define EMBEDPAGE_H

#include "iresponse.h"
#include "playerconfig.h"
#include "videos/videoid.h"
#include <QLoggingCategory>
#include <QNetworkAccessManager>

Q_DECLARE_LOGGING_CATEGORY(ytEmbedPage)

namespace YouTube {
namespace Responses {

class EmbedPage : public IResponse
{
    Q_OBJECT

public:
    explicit EmbedPage(const QByteArray& raw, QObject *parent = nullptr);

    static EmbedPage* get(QNetworkAccessManager *networkManager, const Videos::VideoId& videoId, QObject *parent = nullptr);

    void parse(const QByteArray& raw);

    PlayerConfig* playerConfig() const { return m_playerConfig; }

private:
    explicit EmbedPage(QObject *parent = nullptr) : IResponse(parent) {}

    PlayerConfig* m_playerConfig = nullptr;
};

}
}

#endif // EMBEDPAGE_H
