#include "youtubeutils.h"

#include <QRegularExpression>

bool YouTubeUtils::validateVideoId(QString videoId)
{
    if (videoId.isEmpty()) return false;

    // Video IDs are always 11 characters
    if (videoId.length() != 11) return false;

    QRegularExpression regex("[^0-9a-zA-Z_\\-]");
    auto match = regex.match(videoId);

    return !match.hasMatch();
}

bool YouTubeUtils::tryParseVideoId(QString videoUrl, QString *videoId)
{
    if (videoUrl.isEmpty()) return false;

    // https://www.youtube.com/watch?v=yIVRs6YSbOM
    QRegularExpression regularExp("youtube\\..+?/watch.*?v=(.*?)(?:&|/|$)");
    auto regularMatch = regularExp.match(videoUrl);

    if (regularMatch.hasMatch() && !regularMatch.captured(1).isEmpty() && validateVideoId(regularMatch.captured(1)))
    {
        *videoId = regularMatch.captured(1);
        return true;
    }

    // https://youtu.be/yIVRs6YSbOM
    QRegularExpression shortExp("youtu\\.be/(.*?)(?:\\?|&|/|$)");
    auto shortMatch = shortExp.match(videoUrl);

    if (shortMatch.hasMatch() && !shortMatch.captured(1).isEmpty() && validateVideoId(shortMatch.captured(1)))
    {
        *videoId = shortMatch.captured(1);
        return true;
    }

    // https://www.youtube.com/embed/yIVRs6YSbOM
    QRegularExpression embedExp("youtube\\..+?/embed/(.*?)(?:\\?|&|/|$)");
    auto embedMatch = embedExp.match(videoUrl);

    if (embedMatch.hasMatch() && !embedMatch.captured(1).isEmpty() && validateVideoId(embedMatch.captured(1)))
    {
        *videoId = embedMatch.captured(1);
        return true;
    }

    return false;
}

QString YouTubeUtils::parseVideoId(QString videoUrl)
{
    QString videoId;

    tryParseVideoId(videoUrl, &videoId);
    return videoId;
}

bool YouTubeUtils::validatePlaylistId(QString playlistId)
{
    if (playlistId.isEmpty()) return false;

    // Watch later playlist is special
    if (playlistId == "WL") return true;

    // Other playlist IDs should start with these two characters
    if (!playlistId.startsWith("PL", Qt::CaseInsensitive) &&
        !playlistId.startsWith("RD", Qt::CaseInsensitive) &&
        !playlistId.startsWith("UL", Qt::CaseInsensitive) &&
        !playlistId.startsWith("UU", Qt::CaseInsensitive) &&
        !playlistId.startsWith("PU", Qt::CaseInsensitive) &&
        !playlistId.startsWith("OL", Qt::CaseInsensitive) &&
        !playlistId.startsWith("LL", Qt::CaseInsensitive) &&
        !playlistId.startsWith("FL", Qt::CaseInsensitive)) return false;

    // Playlist IDs vary a lot in lengths, so we will just compare with the
    // extremes
    if ((playlistId.length() < 13) || (playlistId.length() > 42)) return false;

    QRegularExpression regex("[^0-9a-zA-Z_\\-]");
    auto match = regex.match(playlistId);

    return !match.hasMatch();
}

bool YouTubeUtils::tryParsePlaylistId(QString playlistUrl, QString *playlistId)
{
    if (playlistUrl.isEmpty()) return false;

    // https://www.youtube.com/playlist?list=PLOU2XLYxmsIJGErt5rrCqaSGTMyyqNt2H
    QRegularExpression regularExp("youtube\\..+?/playlist.*?list=(.*?)(?:&|/|$)");
    auto regularMatch = regularExp.match(playlistUrl);

    if (regularMatch.hasMatch() && !regularMatch.captured(1).isEmpty() && validateVideoId(regularMatch.captured(1)))
    {
        *playlistId = regularMatch.captured(1);
        return true;
    }

    // https://www.youtube.com/watch?v=b8m9zhNAgKs&list=PL9tY0BWXOZFuFEG_GtOBZ8-8wbkH-NVAr
    QRegularExpression compositeExp("youtube\\..+?/watch.*?list=(.*?)(?:&|/|$)");
    auto compositeMatch = compositeExp.match(playlistUrl);

    if (compositeMatch.hasMatch() && !compositeMatch.captured(1).isEmpty() && validateVideoId(compositeMatch.captured(1)))
    {
        *playlistId = compositeMatch.captured(1);
        return true;
    }

    // https://youtu.be/b8m9zhNAgKs/?list=PL9tY0BWXOZFuFEG_GtOBZ8-8wbkH-NVAr
    QRegularExpression shortCompositeExp("youtube\\..+?/embed/(.*?)(?:\\?|&|/|$)");
    auto shortCompositeMatch = shortCompositeExp.match(playlistUrl);

    if (shortCompositeMatch.hasMatch() && !shortCompositeMatch.captured(1).isEmpty() && validateVideoId(shortCompositeMatch.captured(1)))
    {
        *playlistId = shortCompositeMatch.captured(1);
        return true;
    }

    // https://www.youtube.com/embed/b8m9zhNAgKs/?list=PL9tY0BWXOZFuFEG_GtOBZ8-8wbkH-NVAr
    QRegularExpression embedCompositeExp("youtube\\..+?/embed/(.*?)(?:\\?|&|/|$)");
    auto embedCompositeMatch = embedCompositeExp.match(playlistUrl);

    if (embedCompositeMatch.hasMatch() && !embedCompositeMatch.captured(1).isEmpty() && validateVideoId(embedCompositeMatch.captured(1)))
    {
        *playlistId = embedCompositeMatch.captured(1);
        return true;
    }

    return false;
}

QString YouTubeUtils::parsePlaylistId(QString playlistUrl)
{
    QString playlistId;

    tryParseVideoId(playlistUrl, &playlistId);
    return playlistId;
}

bool YouTubeUtils::validateUsername(QString username)
{
    if (username.isEmpty()) return false;

    // Usernames can't be longer than 20 characters
    if (username.length() > 20) return false;

    QRegularExpression regex("[^0-9a-zA-Z]");
    auto match = regex.match(username);

    return !match.hasMatch();
}

bool YouTubeUtils::tryParseUsername(QString userUrl, QString *username)
{
    if (userUrl.isEmpty()) return false;

    // https://www.youtube.com/user/TheTyrrr
    QRegularExpression regex("youtube\\..+?/user/(.*?)(?:\\?|&|/|$)");
    auto match = regex.match(username);

    if (match.hasMatch() && !match.captured(1).isEmpty() && validateUsername(match.captured(1)))
    {
        *username = match.captured(1);
        return true;
    }

    return false;
}

QString YouTubeUtils::parseUsername(QString userUrl)
{
    QString username;

    tryParseVideoId(userUrl, &username);
    return username;
}

bool YouTubeUtils::validateChannelId(QString channelId)
{
    if (channelId.isEmpty()) return false;

    // Channel IDs should start with these characters
    if (!channelId.startsWith("UC", Qt::CaseInsensitive)) return false;

    // Channel IDs are always 24 characters
    if (channelId.length() != 24) return false;

    QRegularExpression regex("[^0-9a-zA-Z_\\-]");
    auto match = regex.match(channelId);

    return !match.hasMatch();
}

bool YouTubeUtils::tryParseChannelId(QString channelUrl, QString *channelId)
{
    if (channelUrl.isEmpty()) return false;

    // https://www.youtube.com/channel/UC3xnGqlcL3y-GXz5N3wiTJQ
    QRegularExpression regex("youtube\\..+?/channel/(.*?)(?:\\?|&|/|$)");
    auto match = regex.match(channelUrl);

    if (match.hasMatch() && !match.captured(1).isEmpty() && validateUsername(match.captured(1)))
    {
        *channelId = match.captured(1);
        return true;
    }

    return false;
}

QString YouTubeUtils::parseChannelId(QString channelUrl)
{
    QString channelId;

    tryParseVideoId(channelUrl, &channelId);
    return channelId;
}

QString YouTubeUtils::regexMatch(QString subject, QString expression, QRegularExpression::PatternOption option, int index)
{
    QString value;
    QRegularExpression regex(expression, option);
    auto match = regex.match(subject);

    if (match.hasMatch() && !match.captured(index).isEmpty())
    {
        value = match.captured(index);
    }

    return value;
}

bool YouTubeUtils::regexHasMatch(QString subject, QString expression)
{
    QRegularExpression regex(expression);
    auto match = regex.match(subject);

    return match.hasMatch();
}
