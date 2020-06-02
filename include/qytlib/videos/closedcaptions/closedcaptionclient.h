#ifndef CLOSEDCAPTIONCLIENT_H
#define CLOSEDCAPTIONCLIENT_H

#include <QObject>

// TODO: Implement

namespace YouTube {
namespace Videos {
namespace CC {

class ClosedCaptionClient : public QObject
{
    Q_OBJECT
public:
    explicit ClosedCaptionClient(QObject *parent = nullptr);

signals:

};

}
}
}

#endif // CLOSEDCAPTIONCLIENT_H
