#ifndef CIPHEROPERATION_H
#define CIPHEROPERATION_H

#include <QString>

class CipherOperation
{
public:
    CipherOperation() {}

    virtual ~CipherOperation() {}

    virtual QString decipher(QString input) { return ""; }
};

#endif // CIPHEROPERATION_H
