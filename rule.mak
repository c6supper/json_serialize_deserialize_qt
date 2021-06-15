#---------------------------------------------------------------------------------
# Compiler executables
#---------------------------------------------------------------------------------
CC		:=  $(host)gcc
CXX 		:=  $(host)g++
LD		:=  $(CXX)
MOC		:= @$(QTTOOLDIR)/moc
RCC		:= @$(QTTOOLDIR)/rcc
ASTYLE		:= @astyle
MKDIR		:= @mkdir
COPY          	:= cp -f
COPY_DIR    	:= rsync -Cr
STRIP       	:= $(host)strip
INSTALL_FILE	:= install -m 644 -p
INSTALL_PROGRAM	:= install -m 755 -p
TAR		:= tar
AR		:= @ar


#---------------------------------------------------------------------------------
# Options for code generation
#---------------------------------------------------------------------------------
ifeq (x$(gdb),x1)
DEBUG_FLAGS   += -g3 -O0 -DQPROFILE_VERBOSE_DEBUG_OUTPUT -DDEBUG
endif
DEFINES :=  -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
ifneq (x$(host), x)
CFLAGS		:=	-Ithirdpart/linux/libqjson/include
LDFLAGS		+=	-Lthirdpart/linux/libqjson/lib
ifeq (x$(MAKECMDGOALS), xgui)
CFLAGS		+=	-fPIC
LDFLAGS		+=	-shared
endif
else
DEFINES += -DBUILD_X86
LDFLAGS 	+=	-Lthirdpart/linux/cwidget/install/lib
endif
LDFLAGS 	+=	-L$(QTLIBDIR)

CFLAGS  	+=  -g -Wall $(DEFINES) $(DEBUG_FLAGS) -I$(QTINCDIR) $(INCLUDEDIR)\
	-I$(QTINCDIR)/QtCore -I$(QTINCDIR)/QtGui -I$(QTINCDIR)/QtXml -I$(QTINCDIR)/QtNetwork 
CXXFLAGS	:=  $(CFLAGS)

ASTYLEFLAGS := --options=$(CURDIR)/project/codeformatter.astylerc

#---------------------------------------------------------------------------------
# Any extra libraries you wish to link with your project
#---------------------------------------------------------------------------------
LIBS	+=  -lQtGui -lQtCore -lQtXml -lQtNetwork -lpthread -lqjson

#---------------------------------------------------------------------------------
# build folders
#---------------------------------------------------------------------------------
BUILDDIR   	:=  .build
DEPDIR		:=	.dep
STYLEDIR	:=	.style
MOCDIR		:=	.moc
RCCDIR		:=	.rcc

#---------------------------------------------------------------------------------
# source/header file
#---------------------------------------------------------------------------------
SOURCES 	:= $(shell find $(SOURCEDIR) -name '*.cpp')
HEADERS	:= $(shell find $(SOURCEDIR) -name '*.h')
RCCFILES	:= $(shell find $(SOURCEDIR) -name '*.qrc')
#---------------------------------------------------------------------------------
# moc source file
#---------------------------------------------------------------------------------
MOCS := $(addprefix $(MOCDIR)/,$(HEADERS:%.h=%.moc.cpp))

#---------------------------------------------------------------------------------
# rcc source file
#---------------------------------------------------------------------------------
RCCS := $(addprefix $(RCCDIR)/,$(RCCFILES:%.qrc=%.rcc.cpp))

#---------------------------------------------------------------------------------
# Get list of object files, with paths
#---------------------------------------------------------------------------------
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.cpp=%.o)) $(MOCS:.cpp=.o) $(RCCS:.cpp=.o)

#---------------------------------------------------------------------------------
# Fix dependency destination to be dep relative to the src dir
#---------------------------------------------------------------------------------
DEPENDS := $(addprefix $(DEPDIR)/,$(SOURCES:%.cpp=%.d))

#---------------------------------------------------------------------------------
# Format source file
#---------------------------------------------------------------------------------
STYLES := $(addprefix $(STYLEDIR)/,$(SOURCES:%.cpp=%.cpp.astyle-check-stamp)) $(addprefix $(STYLEDIR)/,$(HEADERS:%.h=%.h.astyle-check-stamp))

