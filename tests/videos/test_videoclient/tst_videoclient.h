#ifndef TST_TESTVIDEOID_H
#define TST_TESTVIDEOID_H

#include <QObject>
#include <QNetworkAccessManager>
#include <qytlib/videos/videoclient.h>

class TestVideoClient : public QObject
{
    Q_OBJECT

public:
    explicit TestVideoClient() : QObject() {}
    ~TestVideoClient() {}

private:
    QNetworkAccessManager *networkManager = nullptr;
    YouTube::Videos::VideoClient *client = nullptr;

private slots:
    void initTestCase();

    void getCorrectMetadata();

    void getAnyMetadata_data();
    void getAnyMetadata();
};

#endif // TST_TESTVIDEOID_H
