#include "qytlib/reverseengineering/responses/playerconfig.h"
#include "qytlib/reverseengineering/responses/streaminfovir.h"

using namespace YouTube::Responses;

PlayerConfig::PlayerConfig(const QJsonObject& root, QObject *parent) : IResponse(parent)
{
    parse(root);
}

void PlayerConfig::parse(const QJsonObject &root)
{
    m_root = root;
    m_args = m_root["args"].toObject();

    loadPlayerResponse();
    loadMuxedStreams();
    loadAdaptiveStreams();
    emit ready();
}

QString PlayerConfig::playerSourceUrl() const
{
    auto assets = m_root["assets"].toObject();
    return QString("https://youtube.com%1").arg(assets["js"].toString());
}

void PlayerConfig::loadPlayerResponse()
{
    auto playerResponse = QJsonDocument::fromJson(m_args["player_response"].toString().toUtf8()).object();
    m_playerResponse = new PlayerResponse(playerResponse, this);
}

void PlayerConfig::loadMuxedStreams()
{
    auto infos = m_args["url_encoded_fmt_stream_map"].toString().split(',');

    for (const auto &info : qAsConst(infos))
    {
        m_muxedStreams.append(new StreamInfoVIR(QUrlQuery(info), this));
    }
}

void PlayerConfig::loadAdaptiveStreams()
{
    auto infos = m_args["adaptive_fmts"].toString().split(',');

    for (const auto &info : qAsConst(infos))
    {
        m_adaptiveStreams.append(new StreamInfoVIR(QUrlQuery(info), this));
    }
}
