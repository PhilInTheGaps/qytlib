#include <QtTest>
#include "tst_stringutils.h"
#include "utils/stringutils.h"

void TestStringUtils::substringUntil_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("until");
    QTest::addColumn<QString>("output");

    QTest::addRow("case 1") << "abc/def/gh"  << "/"  << "abc";
    QTest::addRow("case 2") << "/abc/def/gh" << "/"  << "";
    QTest::addRow("case 3") << "abc/def/gh"  << "c/" << "ab";
    QTest::addRow("case 4") << "abc def gh"  << "/"  << "abc def gh";
}

void TestStringUtils::substringUntil()
{
    QFETCH(QString, input);
    QFETCH(QString, until);
    QFETCH(QString, output);

    QCOMPARE(YouTube::Utils::StringUtils::substringUntil(input, until), output);
}

void TestStringUtils::substringAfter_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("after");
    QTest::addColumn<QString>("output");

    QTest::addRow("case 1") << "abc/def/gh"  << "/"  << "gh";
    QTest::addRow("case 2") << "abc/def/gh/" << "/"  << "";
    QTest::addRow("case 3") << "abc/def/gh"  << "c/" << "def/gh";
    QTest::addRow("case 4") << "abc def gh"  << "/"  << "";
    QTest::addRow("case 5") << "abc def gh"  << ""   << "";
}

void TestStringUtils::substringAfter()
{
    QFETCH(QString, input);
    QFETCH(QString, after);
    QFETCH(QString, output);

    QCOMPARE(YouTube::Utils::StringUtils::substringAfter(input, after), output);
}

QTEST_APPLESS_MAIN(TestStringUtils)
