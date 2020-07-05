#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include "qytlib/channels/channelid.h"

namespace YouTube {
namespace Channels {

class Channel : public QObject
{
    Q_OBJECT

public:
    explicit Channel(QObject *parent = nullptr)
        : QObject(parent) {}
    explicit Channel(const ChannelId& id, QObject *parent = nullptr)
        : QObject(parent), m_channelId(id) {}
    explicit Channel(const ChannelId& id, const QString& title, const QString& logoUrl, QObject *parent = nullptr)
        : QObject(parent), m_channelId(id), m_title(title), m_logoUrl(logoUrl) {}

    operator const QString() const { return QString("Channel (%1)").arg(m_title); }

    /// Channel ID
    ChannelId id() const { return m_channelId; }
    void setId(const QString& id) { m_channelId = id; }

    /// Channel URL
    QString url() const { return QString("https://www.youtube.com/channel/%1").arg(m_channelId); }

    /// Channel Title
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    QString title() const { return m_title; }
    void setTitle(const QString& title) { m_title = title; emit titleChanged(); }

    /// URL of the channel's logo image.
    Q_PROPERTY(QString logoUrl READ logoUrl WRITE setLogoUrl NOTIFY logoUrlChanged)
    QString logoUrl() const { return m_logoUrl; }
    void setLogoUrl(const QString& logoUrl) { m_logoUrl = logoUrl; emit logoUrlChanged(); }

    void setReady() { emit ready(); }

signals:
    void ready();
    void titleChanged();
    void logoUrlChanged();

private:
    ChannelId m_channelId;
    QString m_title;
    QString m_logoUrl;
};

}
}

#endif // CHANNEL_H
