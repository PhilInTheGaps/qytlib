#ifndef PLAYERCONFIG_H
#define PLAYERCONFIG_H

#include <QJsonObject>
#include "playerresponse.h"
#include "iresponse.h"

namespace YouTube {
namespace Responses {

class PlayerConfig : public IResponse
{
    Q_OBJECT

public:
    explicit PlayerConfig(const QJsonObject& root, QObject *parent = nullptr);

    void parse(const QJsonObject& root);

    QString playerSourceUrl() const;

    PlayerResponse* playerResponse() const { return m_playerResponse; }

    QVector<IStreamInfoProvider*> muxedStreams() const { return m_muxedStreams; }

    QVector<IStreamInfoProvider*> adaptiveStreams() const { return m_adaptiveStreams; }

    QVector<IStreamInfoProvider*> streams() const { return muxedStreams() + adaptiveStreams(); }

private:
    explicit PlayerConfig(QObject *parent = nullptr) : IResponse(parent) {}

    QJsonObject m_root;
    QJsonObject m_args;

    PlayerResponse* m_playerResponse = nullptr;
    void loadPlayerResponse();

    QVector<IStreamInfoProvider*> m_muxedStreams;
    void loadMuxedStreams();

    QVector<IStreamInfoProvider*> m_adaptiveStreams;
    void loadAdaptiveStreams();
};

}
}

#endif // PLAYERCONFIG_H
