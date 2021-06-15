TEMPLATE = app
TARGET = qmqtt_example
QT = core network

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += mqtt
} else {
    DEFINES += QMQTT_EXAMPLE
    INCLUDEPATH += ../../../src/mqtt
    LIBS += -L../../../src/mqtt -L../../../src/mqtt/debug -L../../../src/mqtt/release -lqmqtt
}

SOURCES += \
    example.cpp

target.path = $$PWD/../../../install/bin
INSTALLS += target

QTDIR = $$(QTDIR)
isEmpty(QTDIR) {
message("Build for X86")
}
else {
QMAKE_INCDIR_QT=$$(QTDIR)/include
QMAKE_LIBDIR_QT=$$(QTDIR)/lib
QMAKE_MOC=$$(QTDIR)/bin/moc
QMAKE_UIC=$$(QTDIR)/bin/uic 
QMAKE_RCC=$$(QTDIR)/bin/rcc
message("Build for arm")
}