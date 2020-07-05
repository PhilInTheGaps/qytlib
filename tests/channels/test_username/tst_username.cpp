#include <QtTest>
#include "tst_username.h"
#include <qytlib/channels/username.h>

void TestUserName::normalize_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::addRow("valid name 1") << "TheTyrrr" << "TheTyrrr";
    QTest::addRow("valid name 2") << "KannibalenRecords" << "KannibalenRecords";
    QTest::addRow("valid name 3") << "JClayton1994" << "JClayton1994";

    QTest::addRow("invalid name 1") << "" << "";
    QTest::addRow("invalid name 2") << "The_Tyrrr" << "";
    QTest::addRow("invalid name 3") << "0123456789ABCDEFGHIJK" << "";
    QTest::addRow("invalid name 4") << "A1B2C3-" << "";
    QTest::addRow("invalid name 5") << "=0123456789ABCDEF" << "";

    QTest::addRow("valid url 1") << "youtube.com/user/ProZD" << "ProZD";
    QTest::addRow("valid url 2") << "youtube.com/user/TheTyrrr" << "TheTyrrr";

    QTest::addRow("invalid url 1") << "youtube.com/user/P_roZD" << "";
    QTest::addRow("invalid url 2") << "example.com/user/ProZD" << "";
}

void TestUserName::normalize()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    if (QString(QTest::currentDataTag()).startsWith("invalid"))
        QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".*normalize().*"));

    QCOMPARE(YouTube::Channels::UserName::normalize(input), output);
}

void TestUserName::isValid_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("output");

    QTest::addRow("valid 1") << "TheTyrrr" << true;
    QTest::addRow("valid 2") << "KannibalenRecords" << true;
    QTest::addRow("valid 3") << "JClayton1994" << true;
    QTest::addRow("invalid 1") << "" << false;
    QTest::addRow("invalid 2") << "A1B2C3-" << false;
    QTest::addRow("invalid 3") << "=0123456789ABCDEF" << false;
    QTest::addRow("invalid 4") << "The_Tyrrr" << false;
}

void TestUserName::isValid()
{
    QFETCH(QString, input);
    QFETCH(bool, output);

    QCOMPARE(YouTube::Channels::UserName::isValid(input), output);
}

QTEST_APPLESS_MAIN(TestUserName)
