QT += core gui testlib opengl openglwidgets
LIBS += -lglu32 -lopengl32

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += object_parallel_to_source

DEFINES += QT_DEPRECATED_WARNINGS

# Misc. playground files
include($$PWD/code playground/PlaygroundFiles.pri)

# Main and forms
include($$PWD/src/main/main.pri)


# Models
include($$PWD/src/models/models.pri)


# Service and global state
include($$PWD/src/service/service.pri)

# Widgets
include($$PWD/src/widgets/widgets.pri)

# Tests
include($$PWD/tests/tests.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
