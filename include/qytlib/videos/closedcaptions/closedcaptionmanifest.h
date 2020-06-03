#ifndef CLOSEDCAPTIONMANIFEST_H
#define CLOSEDCAPTIONMANIFEST_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QLoggingCategory>

#include "closedcaptiontrackinfo.h"
#include "qytlib/videos/videoid.h"

Q_DECLARE_LOGGING_CATEGORY(ytClosedCaptionManifest);

namespace YouTube {
namespace Videos {
namespace CC {

/// Manifest that contains information about available closed caption tracks in a specific video.
class ClosedCaptionManifest : public QObject
{
    Q_OBJECT
public:
    explicit ClosedCaptionManifest(QObject *parent = nullptr) : QObject(parent) {};

    /// Fetch a ClosedCaptionManifest, emits ready() when finished.
    static ClosedCaptionManifest *get(const VideoId &videoId, QNetworkAccessManager *networkManager, QObject *parent);

    /// Available closed caption tracks
    QList<ClosedCaptionTrackInfo*> tracks() const { return m_tracks; }

    /// Gets the closed caption track in the specified language.
    /// Return nullptr if not found.
    ClosedCaptionTrackInfo *getByLanguage(const QLocale &language) const;

signals:
    void ready();

private:
    QList<ClosedCaptionTrackInfo*> m_tracks;
    void setTracks(QList<ClosedCaptionTrackInfo*> tracks);
};

}
}
}

#endif // CLOSEDCAPTIONMANIFEST_H
