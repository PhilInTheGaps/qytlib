QT += testlib

CONFIG += qt console testcase
CONFIG -= app_bundle

INCLUDEPATH += ../../src
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    tst_cipher.cpp

HEADERS += \
    tst_cipher.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../src/release/ -lqytlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../src/debug/ -lqytlib
else:unix: LIBS += -L$$OUT_PWD/../../../src/ -lqytlib

INCLUDEPATH += $$PWD/../../../src
DEPENDPATH += $$PWD/../../../src

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../src/release/libqytlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../src/debug/libqytlib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../src/release/qytlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../../src/debug/qytlib.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../../src/libqytlib.a
