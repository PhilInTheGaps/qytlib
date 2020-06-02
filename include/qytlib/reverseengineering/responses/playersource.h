#ifndef PLAYERSOURCE_H
#define PLAYERSOURCE_H

#include <QLoggingCategory>
#include <QNetworkAccessManager>

#include "iresponse.h"
#include "qytlib/utils/regexutils.h"
#include "qytlib/reverseengineering/cipher/icipheroperation.h"

Q_DECLARE_LOGGING_CATEGORY(ytPlayerSource);

namespace YouTube {
namespace Responses {

class PlayerSource : public IResponse
{
    Q_OBJECT

public:
    explicit PlayerSource(const QByteArray& root, QObject *parent = nullptr);

    static PlayerSource* get(QNetworkAccessManager *networkManager, const QString& url, QObject *parent = nullptr);

    void parse(const QByteArray& root);

    QString sts() const;

    QVector<Cipher::ICipherOperation*> ciphererOperations() const { return m_cipherOperations; }

private:
    explicit PlayerSource(QObject *parent = nullptr) : IResponse(parent) {}

    QString m_root;
    QString m_deciphererFuncBody;
    QString m_deciphererDefinitionBody;
    QVector<Cipher::ICipherOperation*> m_cipherOperations;

    void loadCipherOperations();
    QString getDeciphererFuncBody() const;
    QString getDeciphererDefinitionBody(const QString &deciphererFuncBody) const;
};

}
}

#endif // PLAYERSOURCE_H
