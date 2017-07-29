#-------------------------------------------------
#
# Project created by QtCreator 2017-05-01T09:06:28
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DProjet
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        window.cpp \
    glwidget.cpp \
    player.cpp

HEADERS  += window.h \
    glwidget.h \
    virtualTrebuchetV5.h \
    player.h

FORMS    += window.ui


INCLUDEPATH+=d:\opencv\opencv\build\includee\opencv \
        d:\opencv\opencv\build\includee\opencv2 \
                   d:\opencv\opencv\build\includee \
        d:\MATLAB\R2014b\extern\include \
        d:\MATLAB\R2014b\extern\include\win64


LIBS += -L"D:\\opencv\\opencv\\build\\x64\\vc12\\lib" \
    -lopencv_core2411 \
    -lopencv_highgui2411 \
    -lopencv_imgproc2411 \
    -lopencv_features2d2411 \
    -lopencv_calib3d2411 \

LIBS+=-LD:\MATLAB\R2014b\extern\lib\win64\microsoft -lmclmcrrt
LIBS+=-LD:\Desktop\semetre4\biblioGra\3DProjet -lvirtualTrebuchetV5

LIBS += -lopengl32
LIBS += -lglu32
LIBS += -lglut64

RESOURCES += \
    texture.qrc
