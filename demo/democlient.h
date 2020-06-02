#ifndef DEMOCLIENT_H
#define DEMOCLIENT_H

#include <QObject>
#include <QNetworkAccessManager>

#include <qytlib/videos/videoclient.h>

class DemoClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title NOTIFY videoInfoChanged)
    Q_PROPERTY(QString author READ author NOTIFY videoInfoChanged)
    Q_PROPERTY(QString description READ description NOTIFY videoInfoChanged)

public:
    explicit DemoClient(QObject *parent = nullptr);

    QString title() const { return m_title; }
    QString author() const { return m_author; }
    QString description() const { return m_description; }

public slots:
    void loadVideo(const QString &url);

signals:
    void videoInfoChanged();

private:
    QNetworkAccessManager *networkManager = nullptr;
    YouTube::Videos::VideoClient *videoClient = nullptr;

    QString m_title;
    QString m_author;
    QString m_description;

private slots:
    void onVideoLoaded();

};

#endif // DEMOCLIENT_H
