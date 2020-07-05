#ifndef TST_USERNAME_H
#define TST_USERNAME_H

#include <QObject>

class TestUserName: public QObject
{
    Q_OBJECT

public:
    TestUserName() {}
    ~TestUserName() {}

private slots:
    void normalize_data();
    void normalize();

    void isValid_data();
    void isValid();
};

#endif // TST_USERNAME_H
