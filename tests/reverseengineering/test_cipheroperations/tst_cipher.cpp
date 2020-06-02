#include <QtTest>

#include "tst_cipher.h"

#include <qytlib/reverseengineering/cipher/reversecipheroperation.h>
#include <qytlib/reverseengineering/cipher/slicecipheroperation.h>
#include <qytlib/reverseengineering/cipher/swapcipheroperation.h>

void TestCipher::reverse_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::addRow("normal") << "abcdef" << "fedcba";
    QTest::addRow("empty") << "" << "";
}

void TestCipher::reverse()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    YouTube::Cipher::ReverseCipherOperation cipher;

    QCOMPARE(cipher.decipher(input), output);
}

void TestCipher::slice_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::addRow("normal") << "abcdef" << "cdef";
    QTest::addRow("empty") << "" << "";
}

void TestCipher::slice()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    YouTube::Cipher::SliceCipherOperation cipher1(2);
    YouTube::Cipher::SliceCipherOperation cipher2(8);

    QCOMPARE(cipher1.decipher(input), output);
    QCOMPARE(cipher2.decipher(input), input);
}

void TestCipher::swap_data()
{
    QTest::addColumn<QString>("input");
    QTest::addColumn<QString>("output");

    QTest::addRow("input") << "abcdef" << "cbadef";
    QTest::addRow("output") << "" << "";
}

void TestCipher::swap()
{
    QFETCH(QString, input);
    QFETCH(QString, output);

    YouTube::Cipher::SwapCipherOperation cipher1(2);
    YouTube::Cipher::SwapCipherOperation cipher2(8);

    QCOMPARE(cipher1.decipher(input), output);
    QCOMPARE(cipher2.decipher(input), input);
}

void TestCipher::decipher()
{
    auto signature = "abcdefg.hijklmop";
    auto solution  = "lkjih.gfeacbd";

    QVector<YouTube::Cipher::ICipherOperation*> operations = {
        new YouTube::Cipher::SwapCipherOperation(3, this), // dbcaefg.hijklmop
        new YouTube::Cipher::ReverseCipherOperation(this), // pmolkjih.gfeacbd
        new YouTube::Cipher::SliceCipherOperation(3, this) // lkjih.gfeacbd
    };

    QCOMPARE(YouTube::Cipher::Utils::decipher(operations, signature), solution);
}

QTEST_APPLESS_MAIN(TestCipher)
