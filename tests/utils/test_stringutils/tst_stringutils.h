#ifndef TST_FILEUTILS_H
#define TST_FILEUTILS_H

#include <QObject>

class TestStringUtils : public QObject
{
    Q_OBJECT

public:
    TestStringUtils() {}
    ~TestStringUtils() {}

private slots:
    void substringUntil_data();
    void substringUntil();

    void substringAfter_data();
    void substringAfter();

};

#endif // TST_FILEUTILS_H
