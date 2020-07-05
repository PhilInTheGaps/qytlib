#ifndef USERNAME_H
#define USERNAME_H

#include <QString>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(ytUserName)

namespace YouTube {
namespace Channels {

class UserName
{
public:
    UserName() {}
    UserName(const QString& nameOrUrl);
    UserName(const char* nameOrUrl) : UserName(QString(nameOrUrl)) {}

    inline operator QString() const { return m_value; }

    static bool isValid(const QString& name);
    static QString normalize(const QString& nameOrUrl);

private:
    QString m_value;
};

inline bool operator==(const UserName& o1, const UserName& o2) { return QString(o1) == QString(o2); }
inline bool operator!=(const UserName& o1, const UserName& o2) { return !(o1 == o2); }

}
}

#endif // USERNAME_H
