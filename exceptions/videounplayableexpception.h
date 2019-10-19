#ifndef VIDEOUNPLAYABLEEXPCEPTION_H
#define VIDEOUNPLAYABLEEXPCEPTION_H

#include <exception>
#include <QObject>

class VideoUnplayableExpception : public std::exception
{
public:
    VideoUnplayableExpception(QString videoId, QString message) : m_videoId(videoId), m_message(message) {}

    const char* what() const noexcept { return m_message.toUtf8(); }

protected:
    QString m_videoId, m_message;
};

#endif // VIDEOUNPLAYABLEEXPCEPTION_H
