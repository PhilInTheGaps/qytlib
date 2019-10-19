#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include "cipheroperation.h"
#include <QObject>

class CipherOperations
{
public:
    static QString decipher(QList<CipherOperation*> &operations, QString signature)
    {
        for (auto operation : operations)
        {
            if (!operation) continue;
            signature = operation->decipher(signature);
        }

        return signature;
    }
};

#endif // EXTENSIONS_H
