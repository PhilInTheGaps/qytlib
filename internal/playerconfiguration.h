#ifndef PLAYERCONFIGURATION_H
#define PLAYERCONFIGURATION_H

#include <QObject>
#include <QUrl>
#include <QDateTime>

class PlayerConfiguration
{
public:
    PlayerConfiguration(QUrl playerSourceUrl, QUrl dashManifestUrl, QUrl hlsManifestUrl,
                        QString muxedStreamInfosUrlEncoded, QString adaptiveStreamInfosUrlEncoded, QDateTime validUntil)
        : m_playerSourceUrl(playerSourceUrl), m_dashManifestUrl(dashManifestUrl), m_hlsManifestUrl(hlsManifestUrl),
          m_muxedStreamInfosUrlEncoded(muxedStreamInfosUrlEncoded), m_adaptiveStreamInfosUrlEncoded(adaptiveStreamInfosUrlEncoded),
          m_validUntil(validUntil) {}

    QUrl playerSourceUrl() const { return m_playerSourceUrl; }

    QUrl dashManifestUrl() const { return m_dashManifestUrl; }

    QUrl hlsManifestUrl() const { return m_hlsManifestUrl; }

    QString muxedStreamInfosUrlEncoded() const { return m_muxedStreamInfosUrlEncoded; }

    QString adaptiveStreamInfosUrlEncoded() const { return m_adaptiveStreamInfosUrlEncoded; }

    QDateTime validUntil() const { return m_validUntil; }

private:
    QUrl m_playerSourceUrl;
    QUrl m_dashManifestUrl;
    QUrl m_hlsManifestUrl;
    QString m_muxedStreamInfosUrlEncoded;
    QString m_adaptiveStreamInfosUrlEncoded;
    QDateTime m_validUntil;
};

#endif // PLAYERCONFIGURATION_H

