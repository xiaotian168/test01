SOURCES += \
    main.cpp \
    mainwindow.cpp \
    chat.cpp

QT += widgets network

win32{
LIBS += -lws2_32
}

HEADERS += \
    mainwindow.h \
    chat.h \
    chatdef.h
