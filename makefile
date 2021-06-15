
#---------------------------------------------------------------------------------
# build variables
#---------------------------------------------------------------------------------
ifndef gdb
export gdb=0
endif

ifndef host
export host=
endif
export host

ifeq (x$(QTDIR), x)
export QTTOOLDIR=$(shell qtchooser -print-env|grep QTTOOLDIR|sed 's/.*=\(.*$$\)/\1/')
export QTLIBDIR=$(shell qtchooser -print-env|grep QTLIBDIR|sed 's/.*=\(.*$$\)/\1/')
export QTINCDIR=/usr/include/qt4
else
export QTTOOLDIR=$(QTDIR)/bin
export QTLIBDIR=$(QTDIR)/lib
export QTINCDIR=$(QTDIR)/include
endif

ifeq ($(PREFIX),) #PREFIX is environment variable, but if it is not set, then set default value
PREFIX := ./install
DESTDIR := ./target
DEBUGDIR := ./debug
endif

#---------------------------------------------------------------------------------
# help
#---------------------------------------------------------------------------------
.PHONY: help
help:
	@echo "=============R-Server build help message============="
	@echo "	make [host=] [release=0/1] [gdb=0/1] engine/gui/remoteexecutor"
	@echo "=============Formate source files============="
	@echo "	make styles"
	@echo "=============Installation============="
	@echo "	make [host=] install"

#---------------------------------------------------------------------------------
# Source folders and executable name
#---------------------------------------------------------------------------------
SOURCEDIR	:= include source/common source/model thirdpart/linux/qprofile thirdpart/linux/client
INCLUDEDIR	:= -Iinclude -Isource/common -Isource/model -Ithirdpart/linux/qmqtt-qt4/install/include/qmqtt \
	-Ithirdpart/linux/qprofile -Ithirdpart/linux/client
LIBS		:= -lqmqtt
LDFLAGS		:= -Lthirdpart/linux/qmqtt-qt4/install/lib
ifeq (x$(MAKECMDGOALS), xgui)
SOURCEDIR	+= source/view
ifeq (x$(host), x)
INCLUDEDIR      += -Ithirdpart/linux/cwidget/install/include
LIBS		+= -lcwidget -lQtWebKit 
endif
endif
ifeq (x$(MAKECMDGOALS), xengine)
SOURCEDIR	+= source/controller
endif
ifeq (x$(MAKECMDGOALS), xremoteexecutor)
SOURCEDIR	+= source/remoteexecutor
endif

#---------------------------------------------------------------------------------
# Build Rule
#---------------------------------------------------------------------------------
include rule.mak

#---------------------------------------------------------------------------------
# Target
#---------------------------------------------------------------------------------
TARGETENGINE	:= $(BUILDDIR)/rserverengine
TARGETENGINELIB	:= $(BUILDDIR)/librserverengine.a
TARGETREMOTEEXECUTOR	:= $(BUILDDIR)/remoteexecutor
ifneq (x$(host), x)
TARGETGUI		:= $(BUILDDIR)/librserverui.so
else
TARGETGUI		:= $(BUILDDIR)/rserverui
endif
TARGETGUILIB		:= $(BUILDDIR)/librserverui.a

#---------------------------------------------------------------------------------
.PHONY: install
#---------------------------------------------------------------------------------
install:
	$(RM) -rf $(PREFIX)
	$(MKDIR) -p $(PREFIX)/debug/bin
	$(MKDIR) -p $(PREFIX)/debug/lib
	$(MKDIR) -p $(PREFIX)/bin/
	$(MKDIR) -p $(PREFIX)/lib/
	$(MKDIR) -p $(PREFIX)/include/
	$(COPY_DIR)  --exclude=.svn --exclude='devicerelatedprofile.json' ./etc $(PREFIX)/ 
	$(INSTALL_PROGRAM) $(TARGETENGINE) $(PREFIX)/bin/
	$(INSTALL_PROGRAM) $(TARGETENGINE) $(PREFIX)/debug/bin
	$(INSTALL_PROGRAM) $(TARGETREMOTEEXECUTOR) $(PREFIX)/bin/
	$(INSTALL_PROGRAM) $(TARGETREMOTEEXECUTOR) $(PREFIX)/debug/bin
	$(INSTALL_FILE) $(TARGETGUI) $(PREFIX)/lib/
	$(INSTALL_FILE) $(TARGETGUI) $(PREFIX)/debug/lib/
	#$(INSTALL_FILE) $(TARGETGUILIB) $(PREFIX)/lib/
	$(INSTALL_FILE) $(TARGETGUILIB) $(PREFIX)/debug/lib/
	#$(INSTALL_FILE) $(TARGETENGINELIB) $(PREFIX)/lib/
	$(INSTALL_FILE) $(TARGETENGINELIB) $(PREFIX)/debug/lib/
	$(COPY) ./include/* $(PREFIX)/include
	$(COPY_DIR) --include '*/' --include '*.h' --exclude '*' ./source/common $(PREFIX)/include
	$(COPY_DIR) --include '*/' --include '*.h' --exclude '*' ./source/view $(PREFIX)/include
	$(COPY_DIR) --include '*/' --include '*.h' --exclude '*' ./source/controller $(PREFIX)/include
	$(COPY_DIR) --include '*/' --include '*.h' --exclude '*' ./source/model $(PREFIX)/include
	$(COPY_DIR) --include '*/' --include '*.h' --exclude '*' ./source/remoteexecutor $(PREFIX)/include
	$(COPY_DIR) --include '*/' --include '*.h' --exclude '*' ./thirdpart/linux/qprofile $(PREFIX)/include
	find $(PREFIX)/include -name *.svn |xargs $(RM) -rf
	$(STRIP) -s $(PREFIX)/bin/*
#	$(STRIP) -s $(PREFIX)/lib/*
	find $(PREFIX)/lib/* |grep -v "\\.a" |xargs $(STRIP) -s
	cd $(PREFIX) && $(TAR) -czf ./rserver.tar.gz ./bin ./lib ./include ./etc 
	cd $(PREFIX) && $(TAR) cf - ./bin ./lib ./include ./etc debug/ | xz -9e > rserver_debug.tar.xz
	#$(COPY) $(PREFIX)/rserver.tar.gz $(DESTDIR)/
	#$(COPY) $(PREFIX)/rserver_debug.tar.xz $(DEBUGDIR)/
#---------------------------------------------------------------------------------
.PHONY: gui
#---------------------------------------------------------------------------------
gui:$(MOCDIR) $(RCCDIR) $(STYLEDIR) $(STYLES) $(DEPDIR) $(BUILDDIR) $(TARGETGUI) $(TARGETGUILIB)

#---------------------------------------------------------------------------------
.PHONY: engine
#---------------------------------------------------------------------------------
engine:$(MOCDIR) $(STYLEDIR) $(STYLES) $(DEPDIR) $(BUILDDIR) $(TARGETENGINE) $(TARGETENGINELIB)

#---------------------------------------------------------------------------------
.PHONY: remoteexecutor
#---------------------------------------------------------------------------------
remoteexecutor:$(MOCDIR) $(STYLEDIR) $(STYLES) $(DEPDIR) $(BUILDDIR) $(TARGETREMOTEEXECUTOR)

#---------------------------------------------------------------------------------
.PHONY: styles
#---------------------------------------------------------------------------------
styles: $(STYLES)

#---------------------------------------------------------------------------------
#all: gui engine
#---------------------------------------------------------------------------------

#---------------------------------------------------------------------------------
.PHONY: clean
#---------------------------------------------------------------------------------
clean:
	$(RM) -rf $(BUILDDIR) $(DEPDIR) $(STYLEDIR) $(MOCDIR) $(RCCDIR)
	@echo "============="

$(BUILDDIR) : 
	$(MKDIR) $(BUILDDIR)
	
$(DEPDIR) :
	$(MKDIR) $(DEPDIR)
	
$(STYLEDIR) :
	$(MKDIR) -p $(STYLEDIR)
	
$(MOCDIR) :
	$(MKDIR) $(MOCDIR)

$(RCCDIR) :
	$(MKDIR) $(RCCDIR)
#---------------------------------------------------------------------------------
$(TARGETGUI): $(DEPENDS) $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)
	@echo "============="

#---------------------------------------------------------------------------------
$(TARGETGUILIB): $(OBJECTS)
	$(RM) $@
	$(AR) -crv $@ $(OBJECTS) > /dev/null
#	$(RM) $(OBJECTS)
	@echo "============="
	
#---------------------------------------------------------------------------------
$(TARGETENGINE): $(DEPENDS) $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)
	@echo "============="

#---------------------------------------------------------------------------------
$(TARGETENGINELIB): $(OBJECTS)
	$(RM) $@
	$(AR) -crv $@ $(OBJECTS) > /dev/null
#	$(RM) $(OBJECTS)
	@echo "============="

#---------------------------------------------------------------------------------
$(TARGETREMOTEEXECUTOR): $(DEPENDS) $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)
	@echo "============="

#---------------------------------------------------------------------------------
$(STYLEDIR)/%.cpp.astyle-check-stamp : %.cpp 
#---------------------------------------------------------------------------------
	$(MKDIR) -p $(dir $@)
	@echo "============="
	$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@
	@echo "============="
	
#---------------------------------------------------------------------------------
$(STYLEDIR)/%.h.astyle-check-stamp : %.h 
#---------------------------------------------------------------------------------
	$(MKDIR) -p $(dir $@)
	@echo "============="
	$(ASTYLE) $(ASTYLEFLAGS) $< && touch $@
	@echo "============="

#---------------------------------------------------------------------------------
$(BUILDDIR)/%.o: %.cpp
#---------------------------------------------------------------------------------
	$(MKDIR) -p $(dir $@)
	@echo "============="
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
	@echo "============="

#---------------------------------------------------------------------------------
$(MOCDIR)/%.moc.cpp: %.h
#---------------------------------------------------------------------------------
	$(MKDIR) -p $(dir $@)
	@echo "============="
	$(MOC) $< $(DEFINES) $(INCLUDE) -o $@
	@echo "============="

#---------------------------------------------------------------------------------
%.moc.o: %.moc.cpp
#---------------------------------------------------------------------------------
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
	@echo "============="

#---------------------------------------------------------------------------------
$(RCCDIR)/%.rcc.cpp: %.qrc
#---------------------------------------------------------------------------------
	$(MKDIR) -p $(dir $@)
	@echo "============="
	$(RCC) $< $(INCLUDE) -o $@
	@echo "============="

#---------------------------------------------------------------------------------
%.rcc.o: %.rcc.cpp
#---------------------------------------------------------------------------------
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@
	@echo "============="

#---------------------------------------------------------------------------------
# dependancy rules
#---------------------------------------------------------------------------------
$(DEPDIR)/%.d: %.cpp
	$(MKDIR) -p $(dir $@)
	@$(SHELL) -ec '$(CC) -M $(CFLAGS) $< | sed "s,\($(notdir $*)\.o\) *:,$(BUILDDIR)/$(dir $<)\1: ," > $@'

#---------------------------------------------------------------------------------        
# Include the dependency files
#---------------------------------------------------------------------------------
-include $(DEPENDS)
