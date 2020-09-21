#include <QtTest>

#include "tst_streamsclient.h"

void TestStreamsClient::initTestCase()
{
    networkManager = new QNetworkAccessManager(this);
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    videoClient = new YouTube::Videos::VideoClient(networkManager, this);
    QVERIFY2(videoClient->streams(), "StreamsClient object is nullptr.");
}

void TestStreamsClient::getManifest_data()
{
    QTest::addColumn<QString>("videoId");
    QTest::addColumn<bool>("shouldWork");

    QTest::addRow("very popular")                       << "9bZkp7q19f0"    << true;
    QTest::addRow("age restricted (embed allowed)")     << "SkRSXFQerZs"    << true;
    QTest::addRow("age restricted (embed not allowed)") << "hySoCSoH-g8"    << true;
    QTest::addRow("embed not allowed (type 1)")         << "_kmeFXjjGfk"    << true;
    QTest::addRow("embed not allowed (type 2)")         << "MeJVWBSsPAY"    << true;
    QTest::addRow("rating not allowed")                 << "5VGm0dczmHc"    << true;
    QTest::addRow("unlisted")                           << "ZGdLIwrGHG8"    << true;
    QTest::addRow("recording of a live stream")         << "rsAAeyAr-9Y"    << true;
    QTest::addRow("has DASH manifest")                  << "AI7ULzgf8RU"    << true;
    QTest::addRow("360° video")                         << "-xNN-bJQ4vI"    << true;

    QTest::addRow("livestream")                         << "5qap5aO4i9A"    << false;
    QTest::addRow("requires purchase")                  << "p3dDcKOFXQg"    << false;
    QTest::addRow("doesn't exist")                      << "qld9w0b-1ao"    << false;
    QTest::addRow("private")                            << "pb_hHv3fByo"    << false;
}

void TestStreamsClient::getManifest()
{
    QFETCH(QString, videoId);
    QFETCH(bool, shouldWork);

    const auto *manifest = videoClient->streams()->getManifest(videoId);

    QSignalSpy spy(manifest, &YouTube::Videos::Streams::StreamManifest::ready);
    QVERIFY2(spy.isValid(), "StreamManifest::ready() is not a valid signal.");
    QVERIFY2(spy.wait(5000), "StreamManifest::read() took too long to receive.");

    if (shouldWork)
    {
        QVERIFY2(!manifest->streams().isEmpty(), "No streams found.");
    }
    else
    {
        QVERIFY2(manifest->streams().isEmpty(), "Streams found, a surprise to be sure...");
    }
}

void TestStreamsClient::getLiveStreamUrl()
{
    auto *url = videoClient->streams()->getLiveStreamUrl(QStringLiteral("5qap5aO4i9A"));

    QSignalSpy spy(url, &YouTube::Videos::Streams::LiveUrl::ready);
    QVERIFY2(spy.isValid(), "LiveUrl::ready() is not a valid signal.");
    QVERIFY2(spy.wait(), "LiveUrl::read() took too long to receive.");

    QVERIFY2(!url->url().isEmpty(), "No livestream url found.");
}

QTEST_GUILESS_MAIN(TestStreamsClient)
