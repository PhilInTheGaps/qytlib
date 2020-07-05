#include <QtTest>
#include "tst_channelid.h"
#include <qytlib/channels/channelid.h>

void TestChannelId::normalize_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::addRow("valid id 1") << "UCEnBXANsKmyj2r9xVyKoDiQ" << "UCEnBXANsKmyj2r9xVyKoDiQ";
    QTest::addRow("valid id 2") << "UC46807r_RiRjH8IU-h_DrDQ" << "UC46807r_RiRjH8IU-h_DrDQ";
    QTest::addRow("valid url 1") << "youtube.com/channel/UC3xnGqlcL3y-GXz5N3wiTJQ" << "UC3xnGqlcL3y-GXz5N3wiTJQ";
    QTest::addRow("valid url 2") << "youtube.com/channel/UCkQO3QsgTpNTsOw6ujimT5Q" << "UCkQO3QsgTpNTsOw6ujimT5Q";
    QTest::addRow("valid url 3") << "youtube.com/channel/UCQtjJDOYluum87LA4sI6xcg" << "UCQtjJDOYluum87LA4sI6xcg";
    QTest::addRow("invalid url 1") << "youtube.com/?channel=UCUC3xnGqlcL3y-GXz5N3wiTJQ" << "";
    QTest::addRow("invalid url 2") << "youtube.com/channel/asd" << "";
    QTest::addRow("invalid url 3") << "youtube.com/" << "";
}

void TestChannelId::normalize()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    if (QString(QTest::currentDataTag()).startsWith("invalid"))
        QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".*normalize().*"));

    QCOMPARE(YouTube::Channels::ChannelId::normalize(input), output);
}

void TestChannelId::isValid_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("output");

    QTest::addRow("valid 1") << "UCEnBXANsKmyj2r9xVyKoDiQ" << true;
    QTest::addRow("valid 2") << "UC46807r_RiRjH8IU-h_DrDQ" << true;
    QTest::addRow("invalid 1") << "" << false;
    QTest::addRow("invalid 2") << "UC3xnGqlcL3y-GXz5N3wiTJ" << false;
    QTest::addRow("invalid 3") << "UC3xnGqlcL y-GXz5N3wiTJQ" << false;
}

void TestChannelId::isValid()
{
    QFETCH(QString, input);
    QFETCH(bool, output);

    QCOMPARE(YouTube::Channels::ChannelId::isValid(input), output);
}

QTEST_APPLESS_MAIN(TestChannelId)
