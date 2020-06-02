#ifndef REVERSECIPHEROPERATION_H
#define REVERSECIPHEROPERATION_H

#include "icipheroperation.h"
#include <algorithm>

namespace YouTube {
namespace Cipher {

class ReverseCipherOperation : public ICipherOperation
{
public:
    explicit ReverseCipherOperation(QObject *parent = nullptr) : ICipherOperation(parent) {}

    QString decipher(QString input) override;

    operator QString() const override { return QString("Reverse"); }
};

}
}



#endif // REVERSECIPHEROPERATION_H
