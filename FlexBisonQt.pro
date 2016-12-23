#-------------------------------------------------
#
# Project created by QtCreator 2016-11-29T08:49:33
#
#-------------------------------------------------
QMAKE_CXXFLAGS += -std=c++11
CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

QMAKE_LEX = flex
QMAKE_YACC = bison
QMAKE_YACCFLAGS = -d -o parser.tab.c -l -v

QT       += core
QT       -= gui

LIBS += -lfl -ly

TARGET = test-lexbiz

TEMPLATE = app

SOURCES += main.cpp \
    myast.cpp \
    symbolstable.cpp

LEXSOURCES = lexer.l
YACCSOURCES = parser.y

HEADERS += \
    myast.h \
    subexpression.h \
    symbolstable.h
