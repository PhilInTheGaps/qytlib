#ifndef DEMOCLIENT_H
#define DEMOCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

#include <qytlib/videos/videoclient.h>

class DemoClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* video READ video NOTIFY videoInfoChanged)
    Q_PROPERTY(QString stream READ stream NOTIFY streamChanged)

public:
    explicit DemoClient(QObject *parent = nullptr);

    QObject *video() const { return m_video; }
    QString stream() const { return m_stream; }

public slots:
    void loadVideo(const QString &url);

signals:
    void videoInfoChanged();
    void streamChanged();

private:
    QNetworkAccessManager *networkManager = nullptr;
    YouTube::Videos::VideoClient *videoClient = nullptr;

    QObject *m_video = nullptr;
    QString m_stream;

private slots:
    void onVideoLoaded();
    void onManifestLoaded();

};

#endif // DEMOCLIENT_H
