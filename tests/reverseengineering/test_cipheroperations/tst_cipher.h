#ifndef TST_CIPHER_H
#define TST_CIPHER_H

#include <QObject>

class TestCipher : public QObject
{
    Q_OBJECT

public:
    TestCipher() {}
    ~TestCipher() {}

private slots:
    void reverse_data();
    void reverse();

    void slice_data();
    void slice();

    void swap_data();
    void swap();

    void decipher();
};

#endif // TST_CIPHER_H
