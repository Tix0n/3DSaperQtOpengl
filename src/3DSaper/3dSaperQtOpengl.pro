QT       += core gui opengl widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

LIBS += -lglut -lGLU -lGL \


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    saper2d.cpp \
    saper3dlogic.cpp \
    saper3dview.cpp \

HEADERS += \
    mainwindow.h \
    cell.h \
    saper2d.h \
    saper3dlogic.h \
    saper3dview.h \


FORMS += \
    mainwindow.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
