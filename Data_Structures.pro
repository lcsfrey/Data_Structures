TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    stringsequencetrie.cpp \
    stringtrie.cpp

HEADERS += \
    binarytree.h \
    stringsequencetrie.h \
    stringtrie.h \
    binaryheap.h \
    linkedlist.h \
    skiplist.h
