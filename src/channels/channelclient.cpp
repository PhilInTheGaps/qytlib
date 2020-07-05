#include "qytlib/channels/channelclient.h"
#include "qytlib/reverseengineering/responses/channelpage.h"
#include "qytlib/reverseengineering/responses/videoinforesponse.h"

Q_LOGGING_CATEGORY(ytChannelClient, "yt.channels.client")

using namespace YouTube::Channels;
using namespace YouTube::Responses;

ChannelClient::ChannelClient(QNetworkAccessManager *networkManager, QObject *parent)
    : QObject(parent), m_networkManager(networkManager)
{

}

Channel *ChannelClient::getChannel(const ChannelId &id)
{
    auto *channel = new Channel(id, this);
    auto *channelPage = ChannelPage::get(m_networkManager, id, channel);

    connect(channelPage, &ChannelPage::ready, channel, [ = ]() {
        channel->setTitle(channelPage->title());
        channel->setLogoUrl(channelPage->logoUrl());
        channel->setReady();

        channelPage->deleteLater();
    });

    return channel;
}

Channel *ChannelClient::getChannelByUser(const QString &userName)
{
    auto *channel = new Channel(this);
    auto *channelPage = ChannelPage::getByName(m_networkManager, userName, channel);

    connect(channelPage, &ChannelPage::ready, channel, [ = ]() {
        channel->setId(channelPage->id());
        channel->setTitle(channelPage->title());
        channel->setLogoUrl(channelPage->logoUrl());
        channel->setReady();

        channelPage->deleteLater();
    });

    return channel;
}

Channel *ChannelClient::getChannelByVideo(const YouTube::Videos::VideoId &videoId)
{
    auto *channel = new Channel(this);
    auto *videoInfoResponse = VideoInfoResponse::get(m_networkManager, videoId, "", this);

    connect(videoInfoResponse, &VideoInfoResponse::ready, channel, [=](){
        auto *playerResponse = videoInfoResponse->playerResponse();
        auto *channelPage = ChannelPage::get(m_networkManager, playerResponse->videoChannelId(), channel);

        connect(channelPage, &ChannelPage::ready, channel, [ = ]() {
            channel->setId(channelPage->id());
            channel->setTitle(channelPage->title());
            channel->setLogoUrl(channelPage->logoUrl());
            channel->setReady();

            channelPage->deleteLater();
        });

        playerResponse->deleteLater();
    });

    return channel;
}
