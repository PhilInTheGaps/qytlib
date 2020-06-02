#include "qytlib/reverseengineering/responses/playersource.h"
#include "qytlib/utils/regexutils.h"

#include "qytlib/reverseengineering/cipher/slicecipheroperation.h"
#include "qytlib/reverseengineering/cipher/swapcipheroperation.h"
#include "qytlib/reverseengineering/cipher/reversecipheroperation.h"

#include <QRegularExpression>
#include <QNetworkReply>

Q_LOGGING_CATEGORY(ytPlayerSource, "yt.responses.player.source");

using namespace YouTube::Responses;
using namespace YouTube::Cipher;

PlayerSource::PlayerSource(const QByteArray &root, QObject *parent)
    : IResponse(parent)
{
    parse(root);
}

PlayerSource *PlayerSource::get(QNetworkAccessManager *networkManager, const QString &url, QObject *parent)
{
    qCDebug(ytPlayerSource()) << "PlayerSource::get()";

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

    m_deciphererFuncBody = getDeciphererFuncBody();
    m_deciphererDefinitionBody = getDeciphererDefinitionBody(m_deciphererFuncBody);
    loadCipherOperations();

    emit ready();
}

QString PlayerSource::sts() const
{
    return Utils::RegExUtils::regexMatch(m_root, "(?<=invalid namespace)(.*?;\\s*=(\\d+);)", 2);
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

    auto statements = m_deciphererFuncBody.split(';');

    // Analyze statements to determine cipher function names
    for (const auto& statement : qAsConst(statements))
    {
        // Get the name of the function called in this statement
        auto calledFuncName = Utils::RegExUtils::regexMatch(statement, "\\w+(?:.|\\[)(\\\"?\\w+(?:\\\")?)\\]?\\(");
        if (calledFuncName.isEmpty()) continue;

        // Slice
        if (Utils::RegExUtils::isMatch(m_deciphererDefinitionBody, QString("%1:\\bfunction\\b\\([a],b\\).(\\breturn\\b)?.?\\w+\\.").arg(QRegularExpression::escape(calledFuncName))))
        {
            auto index = Utils::RegExUtils::regexMatch(statement, "\\(\\w+,(\\d+)\\)").toInt();
            m_cipherOperations.append(new SliceCipherOperation(index, this));
            continue;
        }

        // Swap
        if (Utils::RegExUtils::isMatch(m_deciphererDefinitionBody, QString("%1:\\bfunction\\b\\(\\w+\\,\\w\\).\\bvar\\b.\\bc=a\\b").arg(QRegularExpression::escape(calledFuncName))))
        {
            auto index = Utils::RegExUtils::regexMatch(statement, "\\(\\w+,(\\d+)\\)").toInt();
            m_cipherOperations.append(new SwapCipherOperation(index, this));
            continue;
        }

        // Reverse
        if (Utils::RegExUtils::isMatch(m_deciphererDefinitionBody, QString("%1:\\bfunction\\b\\(\\w+\\)").arg(QRegularExpression::escape(calledFuncName))))
        {
            m_cipherOperations.append(new ReverseCipherOperation(this));
            continue;
        }

        if (calledFuncName != "split" && calledFuncName != "join")
        {
            qCWarning(ytPlayerSource()) << "Error: Unrecognized cipher function" << calledFuncName << "statement:" << statement;
        }
    }
}

QString PlayerSource::getDeciphererFuncBody() const
{
    auto funcName = Utils::RegExUtils::regexMatch(m_root, "(\\w+)=function\\(\\w+\\){(\\w+)=\\2\\.split\\(\\x22{2}\\);.*?return\\s+\\2\\.join\\(\\x22{2}\\)}");
    auto escapedFuncName = QRegularExpression::escape(funcName);

    qCDebug(ytPlayerSource()) << "getDeciphererFuncBody() funcName" << funcName;

    // NOTE: original regex had {{(.*?)\\}} instead of {(.*?)\\}
    // This does not work, not sure, if a C# quirk or not
    return Utils::RegExUtils::regexMatch(m_root, QString("(?!h\\.)%1=function\\(\\w+\\)\\{(.*?)\\}").arg(escapedFuncName));
}

QString PlayerSource::getDeciphererDefinitionBody(const QString& deciphererFuncBody) const
{
    // NOTE:
    // Copied from original, where string did not use @ to ignore escape sequences,
    // hope this is correct
    auto funcName = Utils::RegExUtils::regexMatch(deciphererFuncBody, "(\\w+).\\w+\\(\\w+,\\d+\\);");
    auto escapedFuncName = QRegularExpression::escape(funcName);

    qCDebug(ytPlayerSource()) << "getDeciphererDefinitionBody() funcName" << funcName;

    // NOTE: Had to change the regex pattern, as the original did not work with Qt somehow
    // Original: var\s+{escapedFuncName}=\{{(\w+:function\(\w+(,\w+)?\)\{{(.*?)\}}),?\}};
    return Utils::RegExUtils::regexMatch(m_root, QString("var\\s+%1=\\{(\\w+:function\\(\\w+(,\\w+)?\\)\\{([^\\/]*?)\\}),?\\};").arg(escapedFuncName));
}
