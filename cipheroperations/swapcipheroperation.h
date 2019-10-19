#ifndef SWAPCIPHEROPERATION_H
#define SWAPCIPHEROPERATION_H

#include "cipheroperation.h"

class SwapCipherOperation : public CipherOperation
{
public:
    SwapCipherOperation(int index) : m_index(index) {}

    QString decipher(QString input) override
    {
        QChar temp = input[0];
        input[0] = input[m_index];
        input[m_index] = temp;
        return input;
    }

    QString toString() const { return QString("Swap [%1]").arg(m_index); }

private:
    int m_index;
};

#endif // SWAPCIPHEROPERATION_H
