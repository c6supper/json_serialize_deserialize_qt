greaterThan(QT_MAJOR_VERSION, 4) {
    load(qt_parts)
} else {
    TEMPLATE = subdirs

    SUBDIRS = src 
    #examples

    #examples.depends = src

#    unix:!NO_UNIT_TESTS: {
#        SUBDIRS += tests
#        tests.depends = src
#    }
}

QTDIR = $$(QTDIR)
isEmpty(QTDIR) {
message("Build for X86")
}
else {
QMAKE_INCDIR_QT=QTDIR/include
QMAKE_LIBDIR_QT=QTDIR/lib
QMAKE_MOC=QTDIR/bin/moc
QMAKE_UIC=QTDIR/bin/uic 
QMAKE_RCC=QTDIR/bin/rcc
message("Build for arm")
}

