QT += core gui opengl

TEMPLATE = app

HEADERS += \
    abstractscene.h \
    scene.h \
    window.h

SOURCES += \
    scene.cpp \
    window.cpp \
    main.cpp \
    abstractscene.cpp

OTHER_FILES += \
    basic.vert \
    basic.frag

RESOURCES += \
    resources.qrc
