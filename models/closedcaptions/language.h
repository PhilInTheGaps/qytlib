#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>

class Language
{
public:
    Language(QString code, QString name) : m_code(code), m_name(name) {}

    /**
     * @brief ISO 639-1 code of this language.
     */
    QString code() const { return m_code; }

    /**
     * @brief Full English name of this language.
     */
    QString name() const { return m_name; }

    QString toString() const { return QString("%1 (%2)").arg(m_code).arg(m_name); }

private:
    QString m_code, m_name;
};

#endif // LANGUAGE_H
