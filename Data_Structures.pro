TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += src/main.cpp \
    src/stringsequencetrie.cpp \
    src/stringtrie.cpp

HEADERS += \
    src/binarytree.h \
    src/stringsequencetrie.h \
    src/stringtrie.h \
    src/binaryheap.h \
    src/linkedlist.h \
    src/skiplist.h
