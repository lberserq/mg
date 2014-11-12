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
    shader.cpp \
    tree.cpp \
    branchvis.cpp \
    objectvis.cpp \
    leafvis.cpp

HEADERS += \
    object.h \
    leaf.h \
    shaderprogram.h \
    shaderhelper.h \
    shader.h \
    branch.h \
    tree.h \
    branchvis.h \
    objectvis.h \
    leafvis.h


OTHER_FILES += \
    Vertex.vert \
    TODO.txt

