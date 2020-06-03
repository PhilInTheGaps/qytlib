#ifndef PLAYERRESPONSE_H
#define PLAYERRESPONSE_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDate>
#include <QVariant>
#include <QUrlQuery>
#include <QVector>

#include "iresponse.h"
#include "istreaminfoprovider.h"
#include "qytlib/utils/stringutils.h"
#include "qytlib/videos/closedcaptions/closedcaptiontrackinfo.h"

namespace YouTube {
namespace Responses {

class StreamInfoPR;
class ClosedCaptionTrackInfoPR;

class PlayerResponse : public IResponse
{
    Q_OBJECT

public:
    explicit PlayerResponse(const QJsonObject& root, QObject *parent = nullptr);

    void parse(const QJsonObject& root);

    QString videoPlayabilityStatus() const { return playabilityStatus()["status"].toString(); }

    QString videoPlayabilityError() const { return playabilityStatus()["reason"].toString(); }

    bool isVideoAvailable() const { return QString::compare(videoPlayabilityStatus(), "error", Qt::CaseInsensitive); }

    bool isVideoPlayable() const { return !QString::compare(videoPlayabilityStatus(), "ok", Qt::CaseInsensitive); }

    QString videoTitle() const { return videoDetails()["title"].toString(); }

    QString videoAuthor() const { return videoDetails()["author"].toString(); }

    QDate videoUploadDate() const { return QDate::fromString(microformat()["playerMicroformatRenderer"].toObject()["uploadDate"].toString(), Qt::ISODate); }

    QString videoChannelId() const { return videoDetails()["channelId"].toString(); }

    qint64 videoDuration() const { return videoDetails()["lengthSeconds"].toString().toLongLong(); }

    QStringList keywords() const;

    QString description() const { return videoDetails()["shortDescription"].toString(); }

    qint64 videoViewCount() const { return videoDetails()["viewCount"].toString().toLongLong(); }

    QString previewVideoId() const;

    bool isLive() const { return videoDetails()["isLive"].toBool(); }

    QString dashManifestUrl() const { return streamingData()["dashManifestUrl"].toString(); }

    QString hlsManifestUrl() const { return streamingData()["hlsManifestUrl"].toString(); }

    QVector<IStreamInfoProvider*> muxedStreams() const { return m_muxedStreams; }

    QVector<IStreamInfoProvider*> adaptiveStreams() const { return m_adaptiveStreams; }

    QVector<IStreamInfoProvider*> streams() const { return m_muxedStreams + m_adaptiveStreams; }

    const QVector<Videos::CC::ClosedCaptionTrackInfo*> closedCaptionTracks() const { return m_closedCaptionTracks; }

private:
    QJsonObject m_root;

    QJsonObject playabilityStatus() const { return m_root["playabilityStatus"].toObject(); }
    QJsonObject videoDetails() const { return m_root["videoDetails"].toObject(); }
    QJsonObject microformat() const { return m_root["microformat"].toObject(); }
    QJsonObject streamingData() const { return m_root["streamingData"].toObject(); }

    QVector<IStreamInfoProvider*> m_muxedStreams;
    QVector<IStreamInfoProvider*> m_adaptiveStreams;
    QVector<Videos::CC::ClosedCaptionTrackInfo*> m_closedCaptionTracks;

    void loadMuxedStreams();
    void loadAdaptiveStreams();
    void loadClosedCaptionTracks();
};

class StreamInfoPR : public IStreamInfoProvider
{
    Q_OBJECT

public:
    StreamInfoPR(const QJsonObject& root, QObject *parent = nullptr) : IStreamInfoProvider(parent), m_root(root) {}
    ~StreamInfoPR() override {}

    int tag() const override { return m_root["itag"].toInt(); }

    QString url() const override;

    QString signature() const override;

    QString signatureParameter() const override;

    qint64 contentLength() const override;

    qint64 bitrate() const override { return m_root["bitrate"].toInt(); }

    QString mimeType() const override { return m_root["mimeType"].toString(); }

    QString container() const override;

    bool isAudioOnly() const override { return mimeType().startsWith("audio/", Qt::CaseInsensitive); }

    QString codecs() const override;

    QString audioCodec() const override { return isAudioOnly() ? codecs() : Utils::StringUtils::substringAfter(codecs(), ", "); }

    QString videoCodec() const override { return isAudioOnly() ? "" : Utils::StringUtils::substringUntil(codecs(), ", "); }

    QString videoQualityLabel() const override { return m_root["qualityLabel"].toString(); }

    int videoWidth() const override { return m_root["width"].toInt(); }

    int videoHeight() const override { return m_root["height"].toInt(); }

    int framerate() const override { return m_root["fps"].toInt(); }

private:
    QJsonObject m_root;

    QUrlQuery cipher() const { return QUrlQuery(m_root["cipher"].toString()); }

    QUrlQuery signatureCipher() const { return QUrlQuery(m_root["signatureCipher"].toString()); }
};

class ClosedCaptionTrackInfoPR : public Videos::CC::ClosedCaptionTrackInfo
{
public:
    ClosedCaptionTrackInfoPR(const QJsonObject& root, QObject *parent) : Videos::CC::ClosedCaptionTrackInfo(parent), m_root(root) {}

    QString url() const override { return m_root["baseUrl"].toString(); }

    QLocale language() const override { return QLocale(m_root["languageCode"].toString()); }

    bool isAutoGenerated() const override { return m_root["vssId"].toString().startsWith("a.", Qt::CaseInsensitive); }

private:
    QJsonObject m_root;
};

}
}

#endif // PLAYERRESPONSE_H
