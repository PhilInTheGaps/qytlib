QT       += network

INCLUDEPATH += $$PWD

GUMBO_PATH = $$PWD/lib/QGumboParser/QGumboParser

SUBDIRS += $$PWD/lib/QGumboParser/QGumboParser

SOURCES += \
        $$PWD/internal/heuristics.cpp \
        $$PWD/models/playlist.cpp \
        $$PWD/youtube.cpp \
        $$PWD/youtubeutils.cpp \
        $$GUMBO_PATH/qgumboattribute.cpp \
        $$GUMBO_PATH/qgumbodocument.cpp \
        $$GUMBO_PATH/qgumbonode.cpp \
        $$GUMBO_PATH/gumbo-parser/src/attribute.c \
        $$GUMBO_PATH/gumbo-parser/src/char_ref.c \
        $$GUMBO_PATH/gumbo-parser/src/error.c \
        $$GUMBO_PATH/gumbo-parser/src/parser.c \
        $$GUMBO_PATH/gumbo-parser/src/string_buffer.c \
        $$GUMBO_PATH/gumbo-parser/src/string_piece.c \
        $$GUMBO_PATH/gumbo-parser/src/tag.c \
        $$GUMBO_PATH/gumbo-parser/src/tokenizer.c \
        $$GUMBO_PATH/gumbo-parser/src/utf8.c \
        $$GUMBO_PATH/gumbo-parser/src/util.c \
        $$GUMBO_PATH/gumbo-parser/src/vector.c \
        $$PWD/youtubevideo.cpp

HEADERS += \
        $$PWD/cipheroperations/cipheroperation.h \
        $$PWD/cipheroperations/extensions.h \
        $$PWD/cipheroperations/reversecipheroperation.h \
        $$PWD/cipheroperations/slicecipheroperation.h \
        $$PWD/cipheroperations/swapcipheroperation.h \
        $$PWD/exceptions/unrecognizedstructureexception.h \
        $$PWD/exceptions/videorequirespurchaseexception.h \
        $$PWD/exceptions/videounavailableexception.h \
        $$PWD/exceptions/videounplayableexpception.h \
        $$PWD/internal/heuristics.h \
        $$PWD/internal/playerconfiguration.h \
        $$PWD/models/channel.h \
        $$PWD/models/closedcaptions/closedcaptiontrackinfo.h \
        $$PWD/models/closedcaptions/language.h \
        $$PWD/models/mediastreams/audiostreaminfo.h \
        $$PWD/models/mediastreams/mediastreaminfo.h \
        $$PWD/models/mediastreams/mediastreaminfoset.h \
        $$PWD/models/mediastreams/muxedstreaminfo.h \
        $$PWD/models/mediastreams/videoresolution.h \
        $$PWD/models/mediastreams/videostreaminfo.h \
        $$PWD/models/playlist.h \
        $$PWD/models/statistics.h \
        $$PWD/models/thumbnailset.h \
        $$PWD/models/video.h \
        $$PWD/youtube.h \
        $$PWD/youtubeutils.h \
        $$GUMBO_PATH/qgumboattribute.cpp \
        $$GUMBO_PATH/qgumbodocument.cpp \
        $$GUMBO_PATH/qgumbonode.cpp \
        $$GUMBO_PATH/gumbo-parser/src/attribute.c \
        $$GUMBO_PATH/gumbo-parser/src/char_ref.c \
        $$GUMBO_PATH/gumbo-parser/src/error.c \
        $$GUMBO_PATH/gumbo-parser/src/parser.c \
        $$GUMBO_PATH/gumbo-parser/src/string_buffer.c \
        $$GUMBO_PATH/gumbo-parser/src/string_piece.c \
        $$GUMBO_PATH/gumbo-parser/src/tag.c \
        $$GUMBO_PATH/gumbo-parser/src/tokenizer.c \
        $$GUMBO_PATH/gumbo-parser/src/utf8.c \
        $$GUMBO_PATH/gumbo-parser/src/util.c \
        $$GUMBO_PATH/gumbo-parser/src/vector.c \
        $$GUMBO_PATH/qgumboattribute.h \
        $$GUMBO_PATH/qgumbodocument.h \
        $$GUMBO_PATH/qgumbonode.h \
        $$GUMBO_PATH/gumbo-parser/src/attribute.h \
        $$GUMBO_PATH/gumbo-parser/src/char_ref.h \
        $$GUMBO_PATH/gumbo-parser/src/error.h \
        $$GUMBO_PATH/gumbo-parser/src/gumbo.h \
        $$GUMBO_PATH/gumbo-parser/src/insertion_mode.h \
        $$GUMBO_PATH/gumbo-parser/src/parser.h \
        $$GUMBO_PATH/gumbo-parser/src/string_buffer.h \
        $$GUMBO_PATH/gumbo-parser/src/string_piece.h \
        $$GUMBO_PATH/gumbo-parser/src/token_type.h \
        $$GUMBO_PATH/gumbo-parser/src/tokenizer_states.h \
        $$GUMBO_PATH/gumbo-parser/src/tokenizer.h \
        $$GUMBO_PATH/gumbo-parser/src/utf8.h \
        $$GUMBO_PATH/gumbo-parser/src/util.h \
        $$GUMBO_PATH/gumbo-parser/src/vector.h \
        $$GUMBO_PATH/HtmlTag.h \
        $$PWD/youtubevideo.h

