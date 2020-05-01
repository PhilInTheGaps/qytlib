#include "swapcipheroperation.h"

using namespace YouTube::Cipher;

QString SwapCipherOperation::decipher(QString input)
{
    if (m_index < 0 || m_index > input.length() - 1) return input;

    auto temp = input.at(0);

    input[0] = input[m_index] ;
    input[m_index] = temp;

    return input;
}
