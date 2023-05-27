QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -Wno-deprecated-copy

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    flashcard.cpp \
    flashcardaddnewwindow.cpp \
    flashcardedit.cpp \
    flashcardeditwidget.cpp \
    flashcardeditwindow.cpp \
    flashcardview.cpp \
    imagewidget.cpp \
    leitner.cpp \
    main.cpp \
    mainwindow.cpp \
    snapshot.cpp \
    textwidget.cpp

HEADERS += \
    flashcard.h \
    flashcardaddnewwindow.h \
    flashcardedit.h \
    flashcardeditwidget.h \
    flashcardeditwindow.h \
    flashcardview.h \
    imagewidget.h \
    leitner.h \
    mainwindow.h \
    snapshot.h \
    textwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
