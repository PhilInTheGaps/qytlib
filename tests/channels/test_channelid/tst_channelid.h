#ifndef TST_CHANNELID_H
#define TST_CHANNELID_H

#include <QObject>

class TestChannelId: public QObject
{
    Q_OBJECT

public:
    TestChannelId() {}
    ~TestChannelId() {}

private slots:
    void normalize_data();
    void normalize();

    void isValid_data();
    void isValid();
};

#endif // TST_CHANNELID_H
