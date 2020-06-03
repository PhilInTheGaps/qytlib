#include <QtTest>

#include "tst_videoclient.h"

void TestVideoClient::initTestCase()
{
    networkManager = new QNetworkAccessManager(this);
    networkManager->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);

    client = new YouTube::Videos::VideoClient(networkManager, this);

    QVERIFY2(client->closedCaptions(), "ClosedCaptionClient object is nullptr.");
    QVERIFY2(client->streams(), "StreamsClient object is nullptr.");
}

void TestVideoClient::getCorrectMetadata()
{
    const auto videoUrl = "https://www.youtube.com/watch?v=AI7ULzgf8RU";
    auto *video = client->getVideo(videoUrl);

    QSignalSpy spy(video, &YouTube::Videos::Video::ready);
    QVERIFY2(spy.isValid(), "Video::ready() is not a valid signal.");

    bool isReady = spy.wait(5000);
    QVERIFY2(isReady, "Video::ready() took longer than 5 seconds to be emitted.");

    // General
    QCOMPARE(video->id(), "AI7ULzgf8RU");
    QCOMPARE(video->url(), videoUrl);
    QCOMPARE(video->title(), "Aka no Ha [Another] +HDHR");
    QCOMPARE(video->author(), "Tyrrrz");
    QCOMPARE(video->keywords(), QStringList({ "osu", "mouse", "rhythm game" }));

    // Duration
    int year, month, day;
    video->uploadDate().getDate(&year, &month, &day);
    QCOMPARE(year, 2017);
    QCOMPARE(month, 9);
    QCOMPARE(day, 30);
    QCOMPARE(video->duration(), QTime(0, 0, 0).secsTo(QTime(0, 1, 48)));

    // Thumbnails
    QVERIFY2(video->thumbnails(), "Thumbnailset returns a nullptr!");
    QVERIFY2(!video->thumbnails()->lowResUrl().isEmpty(), "No low resolution thumbnail available.");
    QVERIFY2(!video->thumbnails()->mediumResUrl().isEmpty(), "No medium resolution thumbnail available.");
    QVERIFY2(!video->thumbnails()->highResUrl().isEmpty(), "No high resolution thumbnail available.");
    QVERIFY2(!video->thumbnails()->standardResUrl().isEmpty(), "No standard resolution thumbnail available.");
    QVERIFY2(!video->thumbnails()->maxResUrl().isEmpty(), "No max resolution thumbnail available.");

    // Engagement
    QVERIFY2(video->engagement(), "Engagement returns a nulltr!");
    QVERIFY(video->engagement()->viewCount() >= 134);
    QVERIFY(video->engagement()->likeCount() >= 5);
    QVERIFY(video->engagement()->dislikeCount() >= 0);
}

void TestVideoClient::getAnyMetadata_data()
{
    QTest::addColumn<QString>("videoId");
    QTest::addColumn<bool>("shouldWork");

    QTest::addRow("very popular")           << "9bZkp7q19f0" << true;
    QTest::addRow("age-restricted")         << "SkRSXFQerZs" << true;
    QTest::addRow("rating not allowed")     << "5VGm0dczmHc" << true;
    QTest::addRow("unlisted")               << "ZGdLIwrGHG8" << true;
    QTest::addRow("ongoing live stream")    << "5qap5aO4i9A" << true;

    QTest::addRow("doesn't exist")          << "qld9w0b-1ao" << false;
    QTest::addRow("private")                << "pb_hHv3fByo" << false;
}

void TestVideoClient::getAnyMetadata()
{
    QFETCH(QString, videoId);
    QFETCH(bool, shouldWork);

    if (!shouldWork) QTest::ignoreMessage(QtWarningMsg, QRegularExpression(R"(.*Error:.*)"));

    auto *video = client->getVideo(videoId);

    QSignalSpy spy(video, &YouTube::Videos::Video::ready);
    QVERIFY2(spy.wait(5000), "Video::ready() took longer than 5 seconds to be emitted.");

    if (shouldWork)
    {
        QVERIFY2(video->isAvailable(), "Video seems to be not available.");
    }
    else
    {
        QVERIFY2(!video->isAvailable(), "Video seems to be available unexpectedly.");
    }
}

QTEST_GUILESS_MAIN(TestVideoClient)
