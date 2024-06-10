QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QMAKE_CXXFLAGS += -Wno-deprecated-copy

win32 {
    DESTDIR = D:/qt_projects/EvoMemora_deploy/
    QMAKE_POST_LINK =  windeployqt $$shell_path($$DESTDIR/$${TARGET}.exe)
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    flashcard.cpp \
    flashcardaddnewwindow.cpp \
    flashcardedit.cpp \
    flashcardeditwidget.cpp \
    flashcardeditwindow.cpp \
    flashcardmanager.cpp \
    flashcardview.cpp \
    flashcardviewwidget.cpp \
    homewidget.cpp \
    imagewidget.cpp \
    main.cpp \
    mainwindow.cpp \
    snapshot.cpp \
    textwidget.cpp

HEADERS += \
    credentials.h \
    flashcard.h \
    flashcardaddnewwindow.h \
    flashcardedit.h \
    flashcardeditwidget.h \
    flashcardeditwindow.h \
    flashcardmanager.h \
    flashcardview.h \
    flashcardviewwidget.h \
    homewidget.h \
    imagewidget.h \
    mainwindow.h \
    snapshot.h \
    textwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
