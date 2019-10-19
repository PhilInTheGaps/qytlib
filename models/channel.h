#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QUrl>

class Channel
{
public:
    Channel(QString id, QString title, QString logoUrl) :
        m_id(id), m_title(title), m_logoUrl(logoUrl) {}

    QString id() const { return m_id; }
    QString title() const { return m_title; }
    QUrl logoUrl() const { return m_logoUrl; }

    QString toString() const { return m_title; }

private:
    QString m_id, m_title;
    QUrl m_logoUrl;
};

#endif // CHANNEL_H
