#include <QtTest>
#include "tst_channelclient.h"

void TestChannelClient::initTestCase()
{
    networkManager = new QNetworkAccessManager(this);
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    client = new YouTube::Channels::ChannelClient(networkManager, this);
}

void TestChannelClient::getCorrectMetadata()
{
    const QString channelUrl = "https://www.youtube.com/channel/UCEnBXANsKmyj2r9xVyKoDiQ";
    auto *channel = client->getChannel(channelUrl);

    QSignalSpy spy(channel, &YouTube::Channels::Channel::ready);
    QVERIFY2(spy.isValid(), "Channel::ready() is not a valid signal.");

    bool isReady = spy.wait(5000);
    QVERIFY2(isReady, "Channel::ready() took longer than 5 seconds to be emitted.");

    QCOMPARE(QString(channel->id()), "UCEnBXANsKmyj2r9xVyKoDiQ");
    QCOMPARE(channel->url(), channelUrl);
    QCOMPARE(channel->title(), QStringLiteral("Tyrrrz"));
    QVERIFY2(!channel->logoUrl().isEmpty(), "Logo URL is empty.");
}

void TestChannelClient::getAnyMetadata_data()
{
    QTest::addColumn<QString>("input");

    QTest::addRow("case 1") << "UC46807r_RiRjH8IU-h_DrDQ";
    QTest::addRow("case 2") << "UCJ6td3C9QlPO9O_J5dF4ZzA";
    QTest::addRow("case 3") << "UCiGm_E4ZwYSHV3bcW1pnSeQ";
}

void TestChannelClient::getAnyMetadata()
{
    QFETCH(QString, input);

    auto *channel = client->getChannel(input);

    QSignalSpy spy(channel, &YouTube::Channels::Channel::ready);
    QVERIFY2(spy.isValid(), "Channel::ready() is not a valid signal.");

    bool isReady = spy.wait(5000);
    QVERIFY2(isReady, "Channel::ready() took longer than 5 seconds to be emitted.");

    QCOMPARE(QString(channel->id()), input);
}

void TestChannelClient::getAnyMetadataByVideo_data()
{
    QTest::addColumn<QString>("videoId");
    QTest::addColumn<QString>("channelId");

    QTest::addRow("case 1") << "5NmxuoNyDss" << "UCEnBXANsKmyj2r9xVyKoDiQ";
}

void TestChannelClient::getAnyMetadataByVideo()
{
    QFETCH(QString, videoId);
    QFETCH(QString, channelId);

    auto *channel = client->getChannelByVideo(videoId);

    QSignalSpy spy(channel, &YouTube::Channels::Channel::ready);
    QVERIFY2(spy.isValid(), "Channel::ready() is not a valid signal.");

    bool isReady = spy.wait(5000);
    QVERIFY2(isReady, "Channel::ready() took longer than 5 seconds to be emitted.");

    QCOMPARE(QString(channel->id()), channelId);
}

void TestChannelClient::getAnyMetadataByUser_data()
{
    QTest::addColumn<QString>("userName");
    QTest::addColumn<QString>("channelId");

    QTest::addRow("case 1") << "TheTyrrr" << "UCEnBXANsKmyj2r9xVyKoDiQ";
}

void TestChannelClient::getAnyMetadataByUser()
{
    QFETCH(QString, userName);
    QFETCH(QString, channelId);

    auto *channel = client->getChannelByUser(userName);

    QSignalSpy spy(channel, &YouTube::Channels::Channel::ready);
    QVERIFY2(spy.isValid(), "Channel::ready() is not a valid signal.");

    bool isReady = spy.wait(5000);
    QVERIFY2(isReady, "Channel::ready() took longer than 5 seconds to be emitted.");

    QCOMPARE(QString(channel->id()), channelId);
}

QTEST_GUILESS_MAIN(TestChannelClient)
