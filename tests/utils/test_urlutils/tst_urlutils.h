#ifndef TST_URLUTILS_H
#define TST_URLUTILS_H

#include <QObject>

class TestUrlUtils : public QObject
{
    Q_OBJECT
public:
    TestUrlUtils() {}
    ~TestUrlUtils() {}

private slots:
    void setQueryParameter_data();
    void setQueryParameter();

    void setRouteParameter_data();
    void setRouteParameter();
};

#endif // TST_URLUTILS_H
