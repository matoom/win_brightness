QT -= gui

CONFIG += c++17
CONFIG -= app_bundle console

LIBS += -ldxva2
LIBS += -luser32

SOURCES += \
        displayconfig.cpp \
        main.cpp \
        monitor.cpp
HEADERS += \
    displayconfig.h \
    monitor.h \
    version.h

RC_FILE = resources.rc

OTHER_FILES += \
    resources.rc

QMAKE_LFLAGS += -static -static-libgcc -static-libstdc++ -lstdc++

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = brightness
