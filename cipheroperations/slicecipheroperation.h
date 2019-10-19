#ifndef SLICECIPHEROPERATION_H
#define SLICECIPHEROPERATION_H

#include "cipheroperation.h"

class SliceCipherOperation : public CipherOperation
{
public:
    SliceCipherOperation(int index) : m_index(index) {}

    // TODO: Test schreiben
    QString decipher(QString input) override
    {
        return input.right(input.length() - m_index );
    }

private:
    int m_index;
};

#endif // SLICECIPHEROPERATION_H
