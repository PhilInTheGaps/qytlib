#include "bitrate.h"
#include <QtMath>

using namespace YouTube::Videos::Streams;

QString Bitrate::largestWholeNumberSymbol() const
{
    if (qFabs(gigaBitsPerSecond()) >= 1)
        return "Gbit/s";

    if (qFabs(megaBitsPerSecond()) >= 1)
        return "Mbit/s";

    if (qFabs(kiloBitsPerSecond()) >= 1)
        return "Kbit/s";

    return "Bit/s";
}

qreal Bitrate::largestWholeNumberValue() const
{
    if (qFabs(gigaBitsPerSecond()) >= 1)
        return gigaBitsPerSecond();

    if (qFabs(megaBitsPerSecond()) >= 1)
        return megaBitsPerSecond();

    if (qFabs(kiloBitsPerSecond()) >= 1)
        return kiloBitsPerSecond();

    return bitsPerSecond();
}
