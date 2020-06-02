#ifndef CONTAINER_H
#define CONTAINER_H

#include <QObject>

namespace YouTube {
namespace Videos {
namespace Streams {

class Container : public QObject
{
    Q_OBJECT

public:
    explicit Container(const QString& name, QObject *parent = nullptr) : QObject(parent), m_name(name) {}

    operator QString() const { return m_name; }

private:
    QString m_name;
};

inline bool operator==(const Container& o1, const Container& o2) { return QString(o1) == QString(o2); }
inline bool operator!=(const Container& o1, const Container& o2) { return !(o1 == o2); }

}
}
}

#endif // CONTAINER_H
