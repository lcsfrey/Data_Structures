include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

HEADERS +=     teststringtrie.h \
    ../include/stringtrie.h

SOURCES +=     main.cpp \
    ../src/stringtrie.cpp
