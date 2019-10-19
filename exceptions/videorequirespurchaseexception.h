#ifndef VIDEOREQUIRESPURCHASEEXCEPTION_H
#define VIDEOREQUIRESPURCHASEEXCEPTION_H

#include "videounplayableexpception.h"

class VideoRequiresPurchaseException : public VideoUnplayableExpception
{
public:
    VideoRequiresPurchaseException(QString videoId, QString previewVideoId, QString message) :
        VideoUnplayableExpception(videoId, message), m_previewVideoId(previewVideoId) {}

    QString previewVideoId() const { return m_previewVideoId; }

private:
    QString m_previewVideoId;
};

#endif // VIDEOREQUIRESPURCHASEEXCEPTION_H
