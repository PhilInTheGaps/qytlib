#ifndef SLICECIPHEROPERATION_H
#define SLICECIPHEROPERATION_H

#include "icipheroperation.h"

namespace YouTube {
namespace Cipher {

class SliceCipherOperation : public ICipherOperation
{
public:
    explicit SliceCipherOperation(const int& index, QObject *parent = nullptr) : ICipherOperation(parent), m_index(index) {}

    operator QString() const override { return QString("Slice (%1)").arg(m_index); }

    QString decipher(QString input) override;

private:
    int m_index = -1;

};

}
}

#endif // SLICECIPHEROPERATION_H
