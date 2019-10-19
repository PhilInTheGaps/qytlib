#ifndef YOUTTUBEUTILS_H
#define YOUTTUBEUTILS_H

#include <QObject>
#include <QRegularExpression>

class YouTubeUtils
{
public:
    /**
     * @brief Verifies that the given string is syntactically a valid YouTube video ID.
     */
    static bool validateVideoId(QString videoId);

    /**
     * @brief Tries to parse video ID from a YouTube video URL.
     */
    static bool tryParseVideoId(QString videoUrl, QString *videoId);

    /**
     * @brief Parses video ID from a YouTube video URL.
     */
    static QString parseVideoId(QString videoUrl);

    /**
     * @brief Verifies that the given string is syntactically a valid YouTube playlist ID.
     */
    static bool validatePlaylistId(QString playlistId);

    /**
     * @brief Tries to parse playlist ID from a YouTube playlist URL.
     */
    static bool tryParsePlaylistId(QString playlistUrl, QString *playlistId);

    /**
     * @brief Parses playlist ID from a YouTube playlist URL.
     */
    static QString parsePlaylistId(QString playlistUrl);

    /**
     * @brief Verifies that the given string is syntactically a valid YouTube username.
     */
    static bool validateUsername(QString username);

    /**
     * @brief Tries to parse username from a YouTube user URL.
     */
    static bool tryParseUsername(QString userUrl, QString *username);

    /**
     * @brief Parses username from a YouTube user URL.
     */
    static QString parseUsername(QString userUrl);

    /**
     * @brief Verifies that the given string is syntactically a valid YouTube channel ID.
     */
    static bool validateChannelId(QString channelId);

    /**
     * @brief Tries to parse channel ID from a YouTube channel URL.
     */
    static bool tryParseChannelId(QString channelUrl, QString *channelId);

    /**
     * @brief Parses channel ID from a YouTube channel URL.
     */
    static QString parseChannelId(QString channelUrl);

    static QString regexMatch(QString subject, QString expression, QRegularExpression::PatternOption option = QRegularExpression::NoPatternOption, int index = 1);

    static bool regexHasMatch(QString subject, QString expression);
};

#endif // YOUTTUBEUTILS_H
