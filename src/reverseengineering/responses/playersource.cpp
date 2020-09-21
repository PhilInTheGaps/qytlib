#include "qytlib/reverseengineering/responses/playersource.h"
#include "qytlib/utils/regexutils.h"

#include "qytlib/reverseengineering/cipher/slicecipheroperation.h"
#include "qytlib/reverseengineering/cipher/swapcipheroperation.h"
#include "qytlib/reverseengineering/cipher/reversecipheroperation.h"

#include <QRegularExpression>
#include <QNetworkReply>
#include <QFile>

Q_LOGGING_CATEGORY(ytPlayerSource, "yt.responses.player.source");

using namespace YouTube::Responses;
using namespace YouTube::Cipher;
using namespace YouTube::Utils;

PlayerSource::PlayerSource(const QByteArray &root, QObject *parent)
    : IResponse(parent)
{
    parse(root);
}

PlayerSource *PlayerSource::get(QNetworkAccessManager *networkManager, const QString &url, QObject *parent)
{
    qCDebug(ytPlayerSource()) << "PlayerSource::get()";

    qCDebug(ytPlayerSource()) << "URL:" << url;

    auto *playerSource = new PlayerSource(parent);
    auto *reply = networkManager->get(QNetworkRequest(QUrl(url)));

    connect(reply, &QNetworkReply::finished, playerSource, [ = ]() {
        playerSource->parse(reply->readAll());
        reply->deleteLater();
    });

    return playerSource;
}

void PlayerSource::parse(const QByteArray &root)
{
    qCDebug(ytPlayerSource()) << "PlayerSource::parse()";
    m_root = root;

    QFile file("playersource-dump.js");
    file.open(QIODevice::WriteOnly);
    file.write(root);
    file.close();

    m_deciphererFuncBody = getDeciphererFuncBody();
    m_deciphererDefinitionBody = getDeciphererDefinitionBody(m_deciphererFuncBody);
    loadCipherOperations();

    emit ready();
}

QString PlayerSource::sts() const
{
    // First try
    auto sts = RegExUtils::match(m_root, "this\\.signatureTimestamp=(\\d+)");
    if (!sts.isEmpty()) return sts;

    // Second try
    sts = RegExUtils::match(m_root, "(?<=invalid namespace)(.*?;\\s*=(\\d+);)", 2);
    if (!sts.isEmpty()) return sts;

    qCWarning(ytPlayerSource()) << "Error: could not find sts in player source.";

    return sts;
}

void PlayerSource::loadCipherOperations()
{
    qCDebug(ytPlayerSource()) << "PlayerSource::loadCipherOperations()";

    if (m_deciphererFuncBody.isEmpty())
    {
        qCWarning(ytPlayerSource()) << "Error: Decipherer func body is empty.";
        return;
    }

    if (m_deciphererDefinitionBody.isEmpty())
    {
        qCWarning(ytPlayerSource()) << "Error: Decipherer definition body is empty.";
        return;
    }

    qCDebug(ytPlayerSource()) << "m_deciphererFuncBody" << m_deciphererFuncBody;
    qCDebug(ytPlayerSource()) << "m_deciphererDefinitionBody" << m_deciphererDefinitionBody;

    const auto statements = m_deciphererFuncBody.split(';');

    // Analyze statements to determine cipher function names
    for (const auto& statement : statements)
    {
        // Get the name of the function called in this statement
        auto calledFuncName = RegExUtils::match(statement, "\\w+(?:.|\\[)(\\\"?\\w+(?:\\\")?)\\]?\\(");
        if (calledFuncName.isEmpty()) continue;

        // Slice
        if (RegExUtils::isMatch(m_deciphererDefinitionBody, QString("%1:\\bfunction\\b\\([a],b\\).(\\breturn\\b)?.?\\w+\\.").arg(QRegularExpression::escape(calledFuncName))))
        {
            auto index = RegExUtils::match(statement, "\\(\\w+,(\\d+)\\)").toInt();
            m_cipherOperations.append(new SliceCipherOperation(index, this));
            continue;
        }

        // Swap
        if (RegExUtils::isMatch(m_deciphererDefinitionBody, QString("%1:\\bfunction\\b\\(\\w+\\,\\w\\).\\bvar\\b.\\bc=a\\b").arg(QRegularExpression::escape(calledFuncName))))
        {
            auto index = RegExUtils::match(statement, "\\(\\w+,(\\d+)\\)").toInt();
            m_cipherOperations.append(new SwapCipherOperation(index, this));
            continue;
        }

        // Reverse
        if (RegExUtils::isMatch(m_deciphererDefinitionBody, QString("%1:\\bfunction\\b\\(\\w+\\)").arg(QRegularExpression::escape(calledFuncName))))
        {
            m_cipherOperations.append(new ReverseCipherOperation(this));
            continue;
        }

        if (calledFuncName != "split" && calledFuncName != "join" && calledFuncName != "function")
        {
            qCWarning(ytPlayerSource()) << "Error: Unrecognized cipher function" << calledFuncName << "statement:" << statement;
        }
    }
}

QString PlayerSource::getDeciphererFuncBody() const
{
    auto funcName = RegExUtils::match(m_root, "(\\w+)=function\\(\\w+\\){(\\w+)=\\2\\.split\\(\\x22{2}\\);([\\$_\\w]+).\\w+\\(\\w+,\\d+\\).*?return\\s+\\2\\.join\\(\\x22{2}\\)}", 0);
    qCDebug(ytPlayerSource()) << "getDeciphererFuncBody() funcName" << funcName;
    return funcName;
}

QString PlayerSource::getDeciphererDefinitionBody(const QString& body) const
{
    auto objName = RegExUtils::match(body, "([\\$_\\w]+).\\w+\\(\\w+,\\d+\\);", 1);

    if (objName.isEmpty())
    {
        qCWarning(ytPlayerSource()) << "Error: getDeciphererDefinitionBody() objName is empty";
    }

    auto escapedObjName = QRegularExpression::escape(objName);
    auto result = RegExUtils::match(m_root, QString("var\\s+%1=\\{(\\w+:function\\(\\w+(,\\w+)?\\)\\{(.*?)\\}),?\\};").arg(escapedObjName), 0);

    if (result.isEmpty())
    {
        qCWarning(ytPlayerSource()) << "Error: getDeciphererDefinitionBody() definition body is empty";
    }

    return result;
}
