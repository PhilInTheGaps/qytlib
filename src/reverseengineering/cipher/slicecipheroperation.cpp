#include "slicecipheroperation.h"

using namespace YouTube::Cipher;

QString SliceCipherOperation::decipher(QString input)
{
    if (m_index < 0 || m_index > input.length() - 1) return input;

    return input.mid(m_index);
}
