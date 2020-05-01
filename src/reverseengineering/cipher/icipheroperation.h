#ifndef ICIPHEROPERATION_H
#define ICIPHEROPERATION_H

#include <QObject>
#include <QVector>

namespace YouTube {
namespace Cipher {

class ICipherOperation : public QObject
{
    Q_OBJECT

public:
    explicit ICipherOperation(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~ICipherOperation() {}

    virtual operator QString() const = 0;

    virtual QString decipher(QString input) = 0;
};

class Utils
{
public:
    static QString decipher(const QVector<ICipherOperation*>& operations, const QString& signature)
    {
        auto deciphered = signature;

        for (const auto& operation : operations)
        {
            if (!operation) continue;

            deciphered = operation->decipher(deciphered);
        }

        return deciphered;
    }
};

}
}

#endif // ICIPHEROPERATION_H
