#ifndef VIDEOID_H
#define VIDEOID_H

#include <QString>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(ytVideoId)

namespace YouTube {
namespace Videos {

class VideoId
{
public:
    VideoId(const QString& idOrUrl);
    VideoId(const char* idOrUrl) : VideoId(QString(idOrUrl)) {}

    inline operator QString() const { return m_value; }

    static bool isValid(const QString& id);
    static QString normalize(const QString& idOrUrl);

private:
    QString m_value;
};

inline bool operator==(const VideoId& o1, const VideoId& o2) { return QString(o1) == QString(o2); }
inline bool operator!=(const VideoId& o1, const VideoId& o2) { return !(o1 == o2); }

}
}

#endif // VIDEOID_H
