#ifndef VIDEOUNAVAILABLEEXCEPTION_H
#define VIDEOUNAVAILABLEEXCEPTION_H

#include <exception>
#include <QObject>

class VideoUnavailableException : public std::exception
{
public:
    VideoUnavailableException(QString videoId, QString message) : m_videoId(videoId), m_message(message) {}

    const char* what() const noexcept { return m_message.toUtf8(); }

private:
    QString m_videoId, m_message;
};

#endif // VIDEOUNAVAILABLEEXCEPTION_H
