#include "qytlib/videos/streams/filesize.h"
#include <QtMath>

using namespace YouTube::Videos::Streams;

QString FileSize::largestWholeNumberSymbol() const
{
    if (qFabs(totalGigaBytes()) >= 1)
        return "GB";

    if (qFabs(totalMegaBytes()) >= 1)
        return "MB";

    if (qFabs(totalKiloBytes()) >= 1)
        return "KB";

    return "B";
}

qreal FileSize::largestWholeNumberValue() const
{
    if (qFabs(totalGigaBytes()) >= 1)
        return totalGigaBytes();

    if (qFabs(totalMegaBytes()) >= 1)
        return totalMegaBytes();

    if (qFabs(totalKiloBytes()) >= 1)
        return totalKiloBytes();

    return totalBytes();
}
