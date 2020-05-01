QT       += network

TARGET = qytlib
TEMPLATE = lib
CONFIG += staticlib c++11

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/lib/QGumboParser/QGumboParser
DEPENDPATH  += $$PWD/lib/QGumboParser/QGumboParser

SOURCES += \
        $$PWD/common/engagement.cpp \
        $$PWD/reverseengineering/cipher/reversecipheroperation.cpp \
        $$PWD/reverseengineering/cipher/slicecipheroperation.cpp \
        $$PWD/reverseengineering/cipher/swapcipheroperation.cpp \
        $$PWD/reverseengineering/responses/dashmanifest.cpp \
        $$PWD/reverseengineering/responses/embedpage.cpp \
        $$PWD/reverseengineering/responses/playerconfig.cpp \
        $$PWD/reverseengineering/responses/playerresponse.cpp \
        $$PWD/reverseengineering/responses/playersource.cpp \
        $$PWD/reverseengineering/responses/streaminfovir.cpp \
        $$PWD/reverseengineering/responses/videoinforesponse.cpp \
        $$PWD/reverseengineering/responses/watchpage.cpp \
        $$PWD/reverseengineering/heuristics.cpp \
        $$PWD/utils/regexutils.cpp \
        $$PWD/utils/stringutils.cpp \
        $$PWD/utils/urlutils.cpp \
        $$PWD/videos/closedcaptions/closedcaptionclient.cpp \
        $$PWD/videos/streams/bitrate.cpp \
        $$PWD/videos/streams/filesize.cpp \
        $$PWD/videos/streams/streammanifest.cpp \
        $$PWD/videos/streams/streamsclient.cpp \
        $$PWD/videos/videoclient.cpp \
        $$PWD/videos/videoid.cpp

HEADERS += \
        $$PWD/common/engagement.h \
        $$PWD/common/thumbnailset.h \
        $$PWD/reverseengineering/cipher/icipheroperation.h \
        $$PWD/reverseengineering/cipher/reversecipheroperation.h \
        $$PWD/reverseengineering/cipher/slicecipheroperation.h \
        $$PWD/reverseengineering/cipher/swapcipheroperation.h \
        $$PWD/reverseengineering/responses/dashmanifest.h \
        $$PWD/reverseengineering/responses/embedpage.h \
        $$PWD/reverseengineering/responses/istreaminfoprovider.h \
        $$PWD/reverseengineering/responses/playerconfig.h \
        $$PWD/reverseengineering/responses/playerresponse.h \
        $$PWD/reverseengineering/responses/playersource.h \
        $$PWD/reverseengineering/responses/streaminfovir.h \
        $$PWD/reverseengineering/responses/videoinforesponse.h \
        $$PWD/reverseengineering/responses/watchpage.h \
        $$PWD/reverseengineering/heuristics.h \
        $$PWD/reverseengineering/responses/iresponse.h \
        $$PWD/utils/regexutils.h \
        $$PWD/utils/stringutils.h \
        $$PWD/utils/urlutils.h \
        $$PWD/videos/closedcaptions/closedcaptionclient.h \
        $$PWD/videos/closedcaptions/iclosedcaptiontrackinfo.h \
        $$PWD/videos/streams/bitrate.h \
        $$PWD/videos/streams/container.h \
        $$PWD/videos/streams/filesize.h \
        $$PWD/videos/streams/istreaminfo.h \
        $$PWD/videos/streams/streammanifest.h \
        $$PWD/videos/streams/streamsclient.h \
        $$PWD/videos/video.h \
        $$PWD/videos/videoclient.h \
        $$PWD/videos/videoid.h \
        $$PWD/videos/streams/audioonlystreaminfo.h \
        $$PWD/videos/streams/iaudiostreaminfo.h \
        $$PWD/videos/streams/ivideostreaminfo.h \
        $$PWD/videos/streams/muxedstreaminfo.h \
        $$PWD/videos/streams/videoonlystreaminfo.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/lib/QGumboParser/QGumboParser/release/ -lQGumboParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/lib/QGumboParser/QGumboParser/debug/ -lQGumboParser
else:unix: LIBS += -L$$OUT_PWD/lib/QGumboParser/QGumboParser/ -lQGumboParser

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/lib/QGumboParser/QGumboParser/release/libQGumboParser.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/lib/QGumboParser/QGumboParser/debug/libQGumboParser.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/lib/QGumboParser/QGumboParser/release/QGumboParser.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/lib/QGumboParser/QGumboParser/debug/QGumboParser.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/lib/QGumboParser/QGumboParser/libQGumboParser.a
