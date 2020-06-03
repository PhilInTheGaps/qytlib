#ifndef TST_TESTVIDEOID_H
#define TST_TESTVIDEOID_H

#include <QObject>
#include <QNetworkAccessManager>
#include <qytlib/videos/videoclient.h>

class TestClosedCaptionClient : public QObject
{
    Q_OBJECT

public:
    explicit TestClosedCaptionClient() : QObject() {}
    ~TestClosedCaptionClient() {}

private:
    QNetworkAccessManager *networkManager = nullptr;
    YouTube::Videos::VideoClient *videoClient = nullptr;

private slots:
    void initTestCase();

    void getManifest_data();
    void getManifest();

    void get_any_data();
    void get_any();
    void get_specific();

    void writeToDevice_data();
    void writeToDevice();
};

#endif // TST_TESTVIDEOID_H
