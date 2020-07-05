#ifndef CHANNELID_H
#define CHANNELID_H

#include <QString>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(ytChannelId)

namespace YouTube {
namespace Channels {

class ChannelId
{
public:
    ChannelId() {}
    ChannelId(const QString& idOrUrl);
    ChannelId(const char* idOrUrl) : ChannelId(QString(idOrUrl)) {}

    inline operator QString() const { return m_value; }

    static bool isValid(const QString& id);
    static QString normalize(const QString& idOrUrl);

private:
    QString m_value;
};

inline bool operator==(const ChannelId& o1, const ChannelId& o2) { return QString(o1) == QString(o2); }
inline bool operator!=(const ChannelId& o1, const ChannelId& o2) { return !(o1 == o2); }

}
}

#endif // CHANNELID_H
