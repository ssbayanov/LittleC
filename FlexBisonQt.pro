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
    symbolstable/symbolstable.cpp \
    symbolstable/abstractsymboltablerecord.cpp \
    symbolstable/variabletablerecord.cpp \
    symbolstable/functiontablerecord.cpp \
    abstracttree/abstractastnode.cpp \
    abstracttree/abstractvalueastnode.cpp \
    abstracttree/valuenode.cpp \
    abstracttree/referencenode.cpp \
    abstracttree/functiondeclarenode.cpp \
    abstracttree/functioncallnode.cpp \
    abstracttree/functionreturnnode.cpp \
    abstracttree/listnode.cpp \
    abstracttree/ifnode.cpp \
    abstracttree/whilenode.cpp \
    abstracttree/fornode.cpp \
    abstracttree/binarnode.cpp \
    abstracttree/unarynode.cpp \
    abstracttree/labelnode.cpp \
    abstracttree/gotonode.cpp \
    abstracttree/casenode.cpp \
    abstracttree/switchnode.cpp \
    abstracttree/printnode.cpp \
    abstracttree/arrayreferencenode.cpp \
    abstracttree/arraydeclarenode.cpp \
    abstracttree/scannode.cpp \
    abstracttree/structdeclarenode.cpp \
    symbolstable/structtypetablerecord.cpp \
    symbolstable/structtablerecord.cpp \
    abstracttree/structvariabledeclarenode.cpp \
    abstracttree/structreferencenode.cpp \
    abstracttree/variabledeclarationnode.cpp \
    irprint.cpp \
    abstracttree/assignmentnode.cpp \
    subexpression.cpp \
    abstracttree/structvariableassignment.cpp \
    symbolstable/arraytablerecord.cpp \
    abstracttree/arrayassignmentnode.cpp \
    parsersupport.cpp

LEXSOURCES = lexer.l
YACCSOURCES = parser.y

HEADERS += \
    subexpression.h \
    symbolstable/symbolstable.h \
    parser.tab.h \
    symbolstable/abstractsymboltablerecord.h \
    symbolstable/variabletablerecord.h \
    symbolstable/functiontablerecord.h \
    symbolstable/structtypetablerecord.h \
    symbolstable/structtablerecord.h \
    abstracttree/abstractastnode.h \
    abstracttree/abstractvalueastnode.h \
    abstracttree/valuenode.h \
    abstracttree/referencenode.h \
    abstracttree/functiondeclarenode.h \
    abstracttree/functioncallnode.h \
    abstracttree/functionreturnnode.h \
    abstracttree/listnode.h \
    abstracttree/ifnode.h \
    abstracttree/whilenode.h \
    abstracttree/fornode.h \
    abstracttree/binarnode.h \
    abstracttree/unarynode.h \
    abstracttree/labelnode.h \
    abstracttree/gotonode.h \
    abstracttree/casenode.h \
    abstracttree/switchnode.h \
    abstracttree/printnode.h \
    abstracttree/ast.h \
    abstracttree/arrayreferencenode.h \
    abstracttree/arraydeclarenode.h \
    abstracttree/scannode.h \
    abstracttree/structdeclarenode.h \
    abstracttree/structvariabledeclarenode.h \
    abstracttree/structreferencenode.h \
    abstracttree/variabledeclarationnode.h \
    irprint.h \
    abstracttree/assignmentnode.h \
    abstracttree/structvariableassignment.h \
    symbolstable/arraytablerecord.h \
    abstracttree/arrayassignmentnode.h \
    parsersupport.h

