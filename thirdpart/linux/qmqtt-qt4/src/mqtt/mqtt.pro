TARGET = qmqtt
QT = core network

CONFIG += debug

DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII

HEADERS += \
    $$PWD/qmqtt_global.h \
    $$PWD/qmqtt.h \

include(mqtt.pri)

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS

greaterThan(QT_MAJOR_VERSION, 4) {
    load(qt_module)
} else {
    TEMPLATE = lib

    DEFINES += QT_BUILD_MQTT_LIB

    isEmpty(PREFIX) {
        contains(MEEGO_EDITION,harmattan) {
            PREFIX = /usr
        } else:unix:!symbian {
            maemo5 {
                PREFIX = /opt/usr
            } else {
                PREFIX = $$PWD/../../install
            }
        } else {
            PREFIX = $$[QT_INSTALL_PREFIX]
        }
    }

    headers.files = $$HEADERS
    headers.path = $$PREFIX/include/qmqtt
    target.path = $$PREFIX/lib
    INSTALLS += headers target

    OTHER_FILES += qmqtt.pri
}

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
