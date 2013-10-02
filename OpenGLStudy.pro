QT += core opengl widgets quick

TEMPLATE = app

HEADERS += \
    abstractscene.h \
    scene.h \
    window.h \
    quickwindow.h

SOURCES += \
    scene.cpp \
    window.cpp \
    main.cpp \
    abstractscene.cpp \
    quickwindow.cpp

OTHER_FILES += \
    basic.vert \
    basic.frag \
    main.qml

RESOURCES += \
    resources.qrc
