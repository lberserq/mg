TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += opengl
QMAKE_CXXFLAGS += -std=c++11
QMAKE_LFLAGS += -lGL -lGLEW -lglut
SOURCES += main.cpp \
    object.cpp \
    leaf.cpp \
    shaderprogram.cpp \
    branch.cpp \
    shader.cpp

HEADERS += \
    object.h \
    leaf.h \
    shaderprogram.h \
    shaderhelper.h \
    shader.h \
    branch.h


OTHER_FILES += \
    Vertex.vert

