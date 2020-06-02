#ifndef ICLOSEDCAPTIONTRACKINFO_H
#define ICLOSEDCAPTIONTRACKINFO_H

#include <QObject>
#include <QLocale>

namespace YouTube {
namespace Videos {
namespace CC {

class IClosedCaptionTrackInfo : public QObject
{
    Q_OBJECT

public:
    IClosedCaptionTrackInfo(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~IClosedCaptionTrackInfo() {}

    virtual QString url() const = 0;

    virtual QLocale language() const = 0;

    virtual bool isAutoGenerated() const = 0;
};

}
}
}

#endif // ICLOSEDCAPTIONTRACKINFO_H