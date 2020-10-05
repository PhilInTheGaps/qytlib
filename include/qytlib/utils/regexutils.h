#ifndef REGEXUTILS_H
#define REGEXUTILS_H

#include <QRegularExpression>
#include <QString>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(ytUtilsRegEx)

namespace YouTube {
namespace Utils {

class RegExUtils
{
public:
    static QString match(const QString& input, const QString& expression, const int &index = 1, bool singleLine = true);

    static bool isMatch(const QString& input, const QString& expression);
};

}
}

#endif // REGEXUTILS_H
