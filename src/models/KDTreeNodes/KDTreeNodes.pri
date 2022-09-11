!KDTREENODES_PRI {

    CONFIG += KDTREENODES_PRI

    INCLUDEPATH += $$PWD

    include($$PWD/Node/Node.pri)
    include($$PWD/NodeInner/NodeInner.pri)
    include($$PWD/NodeLeaf/NodeLeaf.pri)
}
