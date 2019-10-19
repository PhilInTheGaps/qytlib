#ifndef REVERSECIPHEROPERATION_H
#define REVERSECIPHEROPERATION_H

#include <algorithm>
#include "cipheroperation.h"

class ReverseCipherOperation : public CipherOperation
{
public:
    ReverseCipherOperation() {}

    QString decipher(QString input) override
    {
        auto reverse = input.toStdString();
        std::reverse(reverse.rbegin(), reverse.rend());
        return QString::fromStdString(reverse);
    }

    QString toString() const { return "Reverse"; }
};

#endif // REVERSECIPHEROPERATION_H
