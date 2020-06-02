#include "qytlib/reverseengineering/cipher/reversecipheroperation.h"

using namespace YouTube::Cipher;

QString ReverseCipherOperation::decipher(QString input)
{
    std::reverse(input.begin(), input.end());
    return input;
}
