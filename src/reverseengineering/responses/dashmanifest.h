#ifndef DASHMANIFEST_H
#define DASHMANIFEST_H

#include <QNetworkAccessManager>
#include <QLoggingCategory>

#include "iresponse.h"
#include "istreaminfoprovider.h"
#include "reverseengineering/cipher/icipheroperation.h"
#include "utils/regexutils.h"

Q_DECLARE_LOGGING_CATEGORY(ytDashManifest)

namespace YouTube {
namespace Responses {

class StreamInfoDM;

class DashManifest : public IResponse
{
    Q_OBJECT

public:
    explicit DashManifest(const QByteArray& raw, QObject *parent = nullptr)
        : IResponse(parent) { parse(raw); }

    /// Get DashManifest from url
    static DashManifest* get(QNetworkAccessManager *networkManager, const QString& url, QObject *parent = nullptr);

    /// Get the DashManifest from url but decipher the signature first
    static DashManifest* get(QNetworkAccessManager *networkManager, const QString& url, QVector<Cipher::ICipherOperation*> cipherOperations, QObject *parent = nullptr);

    /// Load the DashManifest by parsing raw XML data
    void parse(const QByteArray& raw);

    /// Get all available streams
    QVector<IStreamInfoProvider*> streams() const { return m_streams; }

private:
    explicit DashManifest(QObject *parent = nullptr) : IResponse(parent) {}

    QVector<IStreamInfoProvider*> m_streams;
};

class StreamInfoDM : public IStreamInfoProvider
{
    Q_OBJECT

public:
    explicit StreamInfoDM(const int& tag, const int& bitrate, const QString& codecs, const int &width,
                          const int &height, const int &framerate, QObject *parent = nullptr);

    int tag() const override { return m_tag; }

    QString url() const override { return m_url; }
    void setUrl(const QString& url) { m_url = url; }

    /// DASH streams don't have signatures
    QString signature() const override { return ""; }

    /// DASH streams don't have signatures
    QString signatureParameter() const override { return ""; }

    qint64 contentLength() const override { return Utils::RegExUtils::regexMatch(url(), R"([/\?]clen[/=](\d+))").toLong(); }

    qint64 bitrate() const override { return m_bitrate; }

    QString mimeType() const override { return m_mimeType; }

    QString container() const override { return QUrl::fromPercentEncoding(Utils::RegExUtils::regexMatch(url(), "mime[/=]\\w*%2F([\\w\\d]*)").toUtf8()); }

    bool isAudioOnly() const override { return m_isAudioOnly; }
    void setIsAudioOnly(const bool& isAudioOnly) { m_isAudioOnly = isAudioOnly; }

    QString codecs() const override { return m_codecs; }

    QString audioCodec() const override { return isAudioOnly() ? codecs() : ""; }

    QString videoCodec() const override { return isAudioOnly() ? "" : codecs(); }

    /// DASH streams don't have a quality label
    QString videoQualityLabel() const override { return ""; }

    int videoWidth() const override { return m_videoWidth; }

    int videoHeight() const override { return m_videoHeight; }

    int framerate() const override { return m_framerate; }

private:
    int m_tag = 0;
    QString m_url;
    qint64 m_bitrate = 0;
    QString m_mimeType;
    bool m_isAudioOnly = false;
    QString m_codecs;
    int m_videoWidth = 0;
    int m_videoHeight = 0;
    int m_framerate = 0;
};

}
}

#endif // DASHMANIFEST_H
