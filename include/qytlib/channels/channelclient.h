#ifndef CHANNELCLIENT_H
#define CHANNELCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QLoggingCategory>

#include "channel.h"
#include "qytlib/videos/videoid.h"

Q_DECLARE_LOGGING_CATEGORY(ytChannelClient)

namespace YouTube {
namespace Channels {

class ChannelClient : public QObject
{
    Q_OBJECT
public:
    ChannelClient(QNetworkAccessManager *networkManager, QObject *parent = nullptr);

    Channel *getChannel(const ChannelId& id);
    Channel *getChannelByUser(const QString& userName);
    Channel *getChannelByVideo(const Videos::VideoId& videoId);

private:
    QNetworkAccessManager *m_networkManager = nullptr;
};

}
}

#endif // CHANNELCLIENT_H
