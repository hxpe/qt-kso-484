load(qttest_p4)

SOURCES += tst_qsharedpointer.cpp \
    forwarddeclaration.cpp \
    forwarddeclared.cpp \
    nontracked.cpp \
    wrapper.cpp

HEADERS += forwarddeclared.h \
    nontracked.h \
    wrapper.h

QT = core

!symbian {
    DEFINES += SRCDIR=\\\"$$PWD/\\\"
}

include(externaltests.pri)
CONFIG += parallel_test

CONFIG+=insignificant_test # QTQAINFRA-428
