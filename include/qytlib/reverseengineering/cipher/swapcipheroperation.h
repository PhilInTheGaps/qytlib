#ifndef SWAPCIPHEROPERATION_H
#define SWAPCIPHEROPERATION_H

#include "icipheroperation.h"

namespace YouTube {
namespace Cipher {

class SwapCipherOperation : public ICipherOperation
{
public:
    explicit SwapCipherOperation(const int& index, QObject *parent = nullptr) : ICipherOperation(parent), m_index(index) {}

    operator QString() const override { return QString("Swap (%1)").arg(m_index); }

    QString decipher(QString input) override;

private:
    int m_index = -1;
};

}
}


#endif // SWAPCIPHEROPERATION_H
