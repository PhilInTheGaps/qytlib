#include "qytlib/utils/regexutils.h"

Q_LOGGING_CATEGORY(ytUtilsRegEx, "yt.utils.regex")

using namespace YouTube::Utils;

QString RegExUtils::match(const QString &input, const QString &expression, const int& index, bool singleLine)
{
    QRegularExpression re(expression);
    if (singleLine)
    {
        re.setPatternOptions(QRegularExpression::PatternOption::DotMatchesEverythingOption);
    }

    if (!re.isValid())
    {
        qCWarning(ytUtilsRegEx()) << "regexMatch() Error: Regular Expression is not valid:" << expression << re.errorString();
        return "";
    }

    auto match = re.match(input, 0, QRegularExpression::MatchType::NormalMatch);

    if (match.hasMatch() && !match.captured(index).isEmpty())
    {
        return match.captured(index);
    }

    return "";
}

bool RegExUtils::isMatch(const QString &input, const QString &expression)
{
    QRegularExpression re(expression);

    if (!re.isValid())
    {
        qCWarning(ytUtilsRegEx()) << "isMatch() Error: Regular Expression is not valid:" << expression << re.errorString();
        return "";
    }

    auto match = re.match(input);

    return match.hasMatch();
}
