#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <QString>

namespace YouTube {
namespace Utils {

class StringUtils
{
public:
    static QString substringUntil(const QString &string, const QString& sub);
    static QString substringAfter(const QString &string, const QString& sub);
};

}
}

#endif // STRINGUTILS_H
