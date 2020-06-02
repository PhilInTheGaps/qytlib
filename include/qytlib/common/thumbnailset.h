#ifndef THUMBNAILSET_H
#define THUMBNAILSET_H

#include "qytlib/videos/videoid.h"
#include <QString>

namespace YouTube {

class ThumbnailSet : public QObject
{
    Q_OBJECT

public:
    ThumbnailSet(const Videos::VideoId& videoId, QObject *parent = nullptr) : QObject(parent), m_videoId(videoId) {}

    /// Low resolution thumbnail URL.
    Q_PROPERTY(QString lowResUrl READ lowResUrl NOTIFY lowResUrlChanged)
    const QString lowResUrl() const { return QString("https://img.youtube.com/vi/%1/default.jpg").arg(m_videoId); }

    /// Medium resolution thumbnail URL.
    Q_PROPERTY(QString mediumResUrl READ mediumResUrl NOTIFY mediumResUrlChanged)
    const QString mediumResUrl() const { return QString("https://img.youtube.com/vi/%1/mqdefault.jpg").arg(m_videoId); }

    /// High resolution thumbnail URL.
    Q_PROPERTY(QString highResUrl READ highResUrl NOTIFY highResUrlChanged)
    const QString highResUrl() const { return QString("https://img.youtube.com/vi/%1/hqdefault.jpg").arg(m_videoId); }

    /// Standard resolution thumbnail URL.
    /// Not always available.
    Q_PROPERTY(QString standardResUrl READ standardResUrl NOTIFY standardResUrlChanged)
    const QString standardResUrl() const { return QString("https://img.youtube.com/vi/%1/sddefault.jpg").arg(m_videoId); }

    /// Max resolution thumbnail URL.
    /// Not always available.
    Q_PROPERTY(QString maxResUrl READ maxResUrl NOTIFY maxResUrlChanged)
    const QString maxResUrl() const { return QString("https://img.youtube.com/vi/%1/maxresdefault.jpg").arg(m_videoId); }

signals:
    void lowResUrlChanged();
    void mediumResUrlChanged();
    void highResUrlChanged();
    void standardResUrlChanged();
    void maxResUrlChanged();

private:
    Videos::VideoId m_videoId;
};

}

#endif // THUMBNAILSET_H
