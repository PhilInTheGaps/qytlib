#include <QtTest>

#include "tst_closedcaptionclient.h"

void TestClosedCaptionClient::initTestCase()
{
    networkManager = new QNetworkAccessManager(this);
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    videoClient = new YouTube::Videos::VideoClient(networkManager, this);
}

void TestClosedCaptionClient::getManifest_data()
{
    QTest::addColumn<QString>("videoId");
    QTest::addRow("video 1") << "_QdPW8JrYzQ";
    QTest::addRow("video 2") << "YltHGKX80Y8";
}

void TestClosedCaptionClient::getManifest()
{
    QFETCH(QString, videoId);

    auto *client = videoClient->closedCaptions();
    auto *manifest = client->getManifest(videoId);

    QSignalSpy spy(manifest, &YouTube::Videos::CC::ClosedCaptionManifest::ready);
    QVERIFY2(spy.isValid(), "ClosedCaptionManifest::ready() is not a valid signal.");

    bool isReady = spy.wait(5000);
    QVERIFY2(isReady, "ClosedCaptionManifest::ready() took longer than 5 seconds to be emitted.");

    QVERIFY2(!manifest->tracks().isEmpty(), "Manifest does not contain any tracks.");
}

void TestClosedCaptionClient::get_any_data()
{
    QTest::addColumn<QString>("videoId");
    QTest::addRow("video 1") << "_QdPW8JrYzQ";
    QTest::addRow("video 2") << "YltHGKX80Y8";
}

void TestClosedCaptionClient::get_any()
{
    QFETCH(QString, videoId);

    auto *client = videoClient->closedCaptions();

    // Get manifest first
    auto *manifest = client->getManifest(videoId);

    QSignalSpy spy(manifest, &YouTube::Videos::CC::ClosedCaptionManifest::ready);
    QVERIFY2(spy.isValid(), "ClosedCaptionManifest::ready() is not a valid signal.");

    bool isManifestReady = spy.wait(5000);
    QVERIFY2(isManifestReady, "ClosedCaptionManifest::ready() took longer than 5 seconds to be emitted.");

    QVERIFY2(!manifest->tracks().isEmpty(), "Manifest does not contain any tracks.");

    // Get track info from manifest
    auto *trackInfo = manifest->tracks().constFirst();
    QVERIFY2(trackInfo, "ClosedCaptionTrackInfo is nullptr.");

    // Get closed caption track
    auto *track = client->get(trackInfo);
    QVERIFY2(track, "ClosedCaptionTrack is nullptr.");

    QSignalSpy spy2(track, &YouTube::Videos::CC::ClosedCaptionTrack::ready);
    QVERIFY2(spy2.isValid(), "ClosedCaptionTrack::ready() is not a valid signal.");

    bool isTrackReady = spy2.wait(5000);
    QVERIFY2(isTrackReady, "ClosedCaptionTrack::ready() took longer than 5 seconds to be emitted.");

    QVERIFY2(!track->captions().isEmpty(), "No closed captions found!");
}

void TestClosedCaptionClient::get_specific()
{
    const auto videoUrl = "https://www.youtube.com/watch?v=YltHGKX80Y8";

    // Get manifest
    auto *manifest = videoClient->closedCaptions()->getManifest(videoUrl);
    QSignalSpy manifestSpy(manifest, &YouTube::Videos::CC::ClosedCaptionManifest::ready);
    QVERIFY2(manifestSpy.wait(), "Manifest took too long to load.");

    // Get track info
    auto *trackInfo = manifest->getByLanguage(QLocale("en"));
    QVERIFY2(trackInfo, "TrackInfo not found for specified language.");

    // Get track
    auto *track = videoClient->closedCaptions()->get(trackInfo);
    QSignalSpy trackSpy(track, &YouTube::Videos::CC::ClosedCaptionTrack::ready);
    QVERIFY2(trackSpy.wait(), "ClosedCaptionTrack took too long to load.");

    // Get caption
    auto *caption = track->getByTime(QTime(0, 0, 0).msecsTo(QTime(0, 10, 41)));
    QVERIFY2(caption, "Caption for specified time not found.");

    // Get caption part
    auto *captionPart = caption->getPartByTime(QTime(0, 0, 0).msecsTo(QTime::fromMSecsSinceStartOfDay(650)));
    QVERIFY2(captionPart, "CaptionPart for specified time not found");

    // Verify data
    QCOMPARE(caption->text(), "know I worked really hard on not doing");
    QCOMPARE(captionPart->text(), " hard");
}

void TestClosedCaptionClient::writeToDevice_data()
{
    QTest::addColumn<QString>("videoId");
    QTest::addRow("video 1") << "_QdPW8JrYzQ";
    QTest::addRow("video 2") << "YltHGKX80Y8";
}

void TestClosedCaptionClient::writeToDevice()
{
    QFETCH(QString, videoId);

    // Setup
    QBuffer buffer;
    QVERIFY(buffer.open(QIODevice::WriteOnly));

    // Get manifest
    auto *manifest = videoClient->closedCaptions()->getManifest(videoId);
    QSignalSpy manifestSpy(manifest, &YouTube::Videos::CC::ClosedCaptionManifest::ready);
    QVERIFY2(manifestSpy.wait(), "Manifest took too long to load.");

    // Get track info
    auto *trackInfo = manifest->tracks().constFirst();
    QVERIFY2(trackInfo, "TrackInfos not available.");

    // Get track
    auto *track = videoClient->closedCaptions()->get(trackInfo);
    QSignalSpy trackSpy(track, &YouTube::Videos::CC::ClosedCaptionTrack::ready);
    QVERIFY2(trackSpy.wait(), "ClosedCaptionTrack took too long to load.");

    // Write to buffer
    QVERIFY2(YouTube::Videos::CC::ClosedCaptionClient::writeToDevice(track, &buffer), "Could not write to buffer.");
    buffer.close();

    // Reading
    QVERIFY(buffer.open(QIODevice::ReadOnly));
    QVERIFY2(!buffer.readAll().isEmpty(), "Buffer is empty, no data was written.");
    buffer.close();
}

QTEST_GUILESS_MAIN(TestClosedCaptionClient)
