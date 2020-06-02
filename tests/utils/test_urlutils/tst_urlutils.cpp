#include <QtTest>
#include "tst_urlutils.h"
#include <qytlib/utils/urlutils.h>

void TestUrlUtils::setQueryParameter_data()
{
    QTest::addColumn<QString>("url");
    QTest::addColumn<QString>("key");
    QTest::addColumn<QString>("value");
    QTest::addColumn<QString>("output");

    QTest::addRow("param does not exist")        << "test.url?a=b"     << "key" << "value" << "test.url?a=b&key=value";
    QTest::addRow("no param exists")             << "test.url"         << "key" << "value" << "test.url?key=value";
    QTest::addRow("no param exists, trailing /") << "test.url/"        << "key" << "value" << "test.url/?key=value";
    QTest::addRow("param exists")                << "test.url?key=old" << "key" << "value" << "test.url?key=value";
}

void TestUrlUtils::setQueryParameter()
{
    QFETCH(QString, url);
    QFETCH(QString, key);
    QFETCH(QString, value);
    QFETCH(QString, output);

    QCOMPARE(YouTube::Utils::UrlUtils::setQueryParameter(url, key, value), output);
}

void TestUrlUtils::setRouteParameter_data()
{
    QTest::addColumn<QString>("url");
    QTest::addColumn<QString>("key");
    QTest::addColumn<QString>("value");
    QTest::addColumn<QString>("output");

    QTest::addRow("param does not exist")        << "test.url/url"     << "key"  << "value" << "test.url/url/key/value";
    QTest::addRow("no param exists")             << "test.url"         << "key"  << "value" << "test.url/key/value";
    QTest::addRow("no param exists, trailing /") << "test.url/"        << "key"  << "value" << "test.url/key/value";
    QTest::addRow("param exists")                << "test.url/key/old" << "key"  << "value" << "test.url/key/value";
}

void TestUrlUtils::setRouteParameter()
{
    QFETCH(QString, url);
    QFETCH(QString, key);
    QFETCH(QString, value);
    QFETCH(QString, output);

    QCOMPARE(YouTube::Utils::UrlUtils::setRouteParameter(url, key, value), output);
}

QTEST_APPLESS_MAIN(TestUrlUtils)
