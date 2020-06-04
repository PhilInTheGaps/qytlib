#ifndef TST_TESTVIDEOID_H
#define TST_TESTVIDEOID_H

#include <QObject>
#include <QNetworkAccessManager>
#include <qytlib/videos/videoclient.h>

class TestStreamsClient : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager *networkManager = nullptr;
    YouTube::Videos::VideoClient *videoClient = nullptr;

private slots:
    void initTestCase();

    void getManifest_data();
    void getManifest();

    void getLiveStreamUrl();
};

#endif // TST_TESTVIDEOID_H
