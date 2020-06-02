#ifndef WATCHPAGE_H
#define WATCHPAGE_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLoggingCategory>
#include <QNetworkAccessManager>

#include "playerconfig.h"
#include "playerresponse.h"
#include "qytlib/videos/videoid.h"

Q_DECLARE_LOGGING_CATEGORY(ytWatchPage)

namespace YouTube {
namespace Responses {

class WatchPage : public QObject
{
    Q_OBJECT

public:
    explicit WatchPage(const QByteArray& raw, QObject *parent = nullptr);

    static WatchPage* get(QNetworkAccessManager *networkManager, const Videos::VideoId& videoId, QObject *parent = nullptr);

    void parse(const QByteArray& raw);

    bool isOk() const { return m_isOk; }

    bool isVideoAvailable() const { return m_isVideoAvailable; }

    qint64 videoLikeCount() const { return m_videoLikeCount; }

    qint64 videoDislikeCount() const { return m_videoDislikeCount; }

    PlayerConfig* playerConfig() const { return m_playerConfig; }

signals:
    void ready();

private:
    explicit WatchPage(QObject *parent = nullptr) : QObject(parent) {}

    bool m_isOk = false;
    bool m_isVideoAvailable = false;

    qint64 m_videoLikeCount = 0;
    qint64 m_videoDislikeCount = 0;

    PlayerConfig* m_playerConfig = nullptr;
};

}
}

#endif // WATCHPAGE_H
