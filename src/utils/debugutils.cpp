#include "qytlib/utils/debugutils.h"

#ifdef QT_DEBUG
#include <QFile>
#include <QDateTime>
#include <QCryptographicHash>
#include <QDebug>
#endif

Q_LOGGING_CATEGORY(ytUtilsDebug, "yt.utils.debug")

using namespace YouTube::Utils;

/// Dump data into a file
bool Debug::dumpToFile(const QString &filename, const QByteArray &data)
{
#ifdef QT_DEBUG
    // Calculate unique hash from current time
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(QDateTime::currentDateTime().toString().toUtf8());
    auto hashString = hash.result().toBase64();

    auto finalFileName = QString("debug-%1-%2").arg(hashString, filename);
    QFile file(finalFileName);
    qCDebug(ytUtilsDebug()) << "Dumping data to file:" << file.fileName();

    if (!file.open(QIODevice::WriteOnly)) return false;

    if (file.write(data) < 0) return false;

    file.close();
#else
    Q_UNUSED(filename);
    Q_UNUSED(data);
#endif
    return true;
}
