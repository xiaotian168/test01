SOURCES += \
    main.cpp \
    MainWindow.cpp \
    Chat.cpp


QT += widgets network

HEADERS += \
    MainWindow.h \
    Chat.h \
    chatdef.h

win32{
LIBS += -lws2_32
}
