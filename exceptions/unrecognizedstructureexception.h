#ifndef UNRECOGNIZEDSTRUCTUREEXCEPTION_H
#define UNRECOGNIZEDSTRUCTUREEXCEPTION_H

#include <exception>
#include <QObject>

class UnrecognizedStructureException : public std::exception
{
public:
    UnrecognizedStructureException(QString message) : m_message(message) {}

    const char* what() const noexcept { return m_message.toUtf8(); }

private:
    QString m_message;
};

#endif // UNRECOGNIZEDSTRUCTUREEXCEPTION_H
