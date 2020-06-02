#ifndef IRESPONSE_H
#define IRESPONSE_H

#include <QObject>

namespace YouTube {
namespace Responses {

class IResponse : public QObject
{
    Q_OBJECT
public:
    explicit IResponse(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void ready();
};

}
}

#endif // IRESPONSE_H
