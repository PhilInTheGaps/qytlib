#include <QtTest>

#include "tst_videoid.h"
#include <qytlib/videos/videoid.h>

void TestVideoId::isValid_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<bool>("output");

    QTest::addRow("valid") << "9bZkp7q19f0" << true;
    QTest::addRow("valid") << "_kmeFXjjGfk" << true;
    QTest::addRow("valid") << "AI7ULzgf8RU" << true;

    QTest::addRow("invalid (empty)")     << ""            << false;
    QTest::addRow("invalid (too short)") << "pI2I2zqzeK"  << false;
    QTest::addRow("invalid (has gap)")   << "pI2I2z zeKg" << false;
}

void TestVideoId::isValid()
{
    QFETCH(QString, input);
    QFETCH(bool, output);

    QCOMPARE(YouTube::Videos::VideoId::isValid(input), output);
}

void TestVideoId::normalize_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::addRow("valid id") << "9bZkp7q19f0" << "9bZkp7q19f0";
    QTest::addRow("valid id") << "_kmeFXjjGfk" << "_kmeFXjjGfk";
    QTest::addRow("valid id") << "AI7ULzgf8RU" << "AI7ULzgf8RU";

    QTest::addRow("invalid id") << ""            << "";
    QTest::addRow("invalid id") << "pI2I2zqzeK"  << "";
    QTest::addRow("invalid id") << "pI2I2z zeKg" << "";

    QTest::addRow("valid url (normal)") << "youtube.com/watch?v=yIVRs6YSbOM" << "yIVRs6YSbOM";
    QTest::addRow("valid url (short)")  << "youtu.be/yIVRs6YSbOM"            << "yIVRs6YSbOM";
    QTest::addRow("valid url (embed)")  << "youtube.com/embed/yIVRs6YSbOM"   << "yIVRs6YSbOM";

    QTest::addRow("invalid url") << "youtube.com/xxx?v=pI2I2zqzeKg" << "";
    QTest::addRow("invalid url") << "youtu.be/watch?v=xxx"          << "";
    QTest::addRow("invalid url") << "youtube.com/embed/"            << "";
}

void TestVideoId::normalize()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    if (QString(QTest::currentDataTag()).startsWith("invalid"))
        QTest::ignoreMessage(QtWarningMsg, QRegularExpression(".*normalize().*"));

    QCOMPARE(YouTube::Videos::VideoId::normalize(input), output);
}

QTEST_APPLESS_MAIN(TestVideoId)
