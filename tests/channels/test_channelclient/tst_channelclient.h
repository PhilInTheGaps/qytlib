#ifndef TST_CHANNELCLIENT_H
#define TST_CHANNELCLIENT_H

#include <QObject>
#include <qytlib/channels/channelclient.h>

class TestChannelClient : public QObject
{
    Q_OBJECT

public:
    TestChannelClient() {}
    ~TestChannelClient() {}

private:
    QNetworkAccessManager *networkManager = nullptr;
    YouTube::Channels::ChannelClient *client = nullptr;

private slots:
    void initTestCase();

    void getCorrectMetadata();

    void getAnyMetadata_data();
    void getAnyMetadata();

    void getAnyMetadataByVideo_data();
    void getAnyMetadataByVideo();

    void getAnyMetadataByUser_data();
    void getAnyMetadataByUser();
};

#endif // TST_CHANNELCLIENT_H
