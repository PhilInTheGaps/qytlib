#ifndef CLOSEDCAPTIONTRACKINFO_H
#define CLOSEDCAPTIONTRACKINFO_H

#include <QObject>
#include <QLocale>

namespace YouTube {
namespace Videos {
namespace CC {

/// YouTube closed caption track metadata.
class ClosedCaptionTrackInfo : public QObject
{
    Q_OBJECT

public:
    ClosedCaptionTrackInfo(const QString &url, const QLocale &language,
                            bool isAutoGenerated, QObject *parent = nullptr)
        : QObject(parent), m_url(url), m_language(language), m_isAutoGenerated(isAutoGenerated) {}

    ClosedCaptionTrackInfo(QObject *parent = nullptr) : QObject(parent) {}

    inline operator QString() const { return QString("CC Track (%1)").arg(QLocale::languageToString(m_language.language())); }

    /// URL of the associated track.
    virtual QString url() const { return m_url; }

    /// Language of the associated track.
    virtual QLocale language() const { return m_language; }

    /// Whether the associated track was automatically generated.
    virtual bool isAutoGenerated() const { return m_isAutoGenerated; }

private:
    QString m_url;
    QLocale m_language;
    bool m_isAutoGenerated;
};

}
}
}

#endif // CLOSEDCAPTIONTRACKINFO_H
