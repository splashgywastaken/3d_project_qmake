!KDTREE_PRI {
    CONFIG += KDTREE_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/../../models/KDTreeNodes/KDTreeNodes.pri)
    include($$PWD/KDTree Utils/KDTreeUtils.pri)

    HEADERS += \
        $$PWD/kdtree.h

    SOURCES += \
        $$PWD/kdtree.cpp
}
