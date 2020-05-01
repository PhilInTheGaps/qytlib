#ifndef TST_TESTVIDEOID_H
#define TST_TESTVIDEOID_H

#include <QObject>

class TestVideoId : public QObject
{
    Q_OBJECT

public:
    explicit TestVideoId() : QObject() {}
    ~TestVideoId() {}

private slots:
    void isValid_data();
    void isValid();

    void normalize_data();
    void normalize();

};

#endif // TST_TESTVIDEOID_H
