#ifndef DEBUGUTILS_H
#define DEBUGUTILS_H

#include <QString>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(ytUtilsDebug)

namespace YouTube {
namespace Utils {

class Debug
{
public:
    static bool dumpToFile(const QString& filename, const QByteArray& data);
};

}
}

#endif // DEBUGUTILS_H
