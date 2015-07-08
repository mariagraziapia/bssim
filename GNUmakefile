# $Id: GNUmakefile 69899 2013-05-17 10:05:33Z gcosmo $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := bssim
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
#  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin
include $(G4INSTALL)/config/binmake.gmk

ifdef G4VERSION_91
CPPFLAGS += -DG4VERSION_91
endif

ifdef G4VERSION_94
CPPFLAGS += -DG4VERSION_94
endif

ifdef G4VERSION_96
CPPFLAGS += -DG4VERSION_96
endif

ifdef G4VERSION_101
CPPFLAGS += -DG4VERSION_101
endif

ifdef G4VERSION_102
CPPFLAGS += -DG4VERSION_102
endif

ifdef G4ANALYSIS_USE  
 CPPFLAGS += `aida-config --include`
 LDFLAGS  += `aida-config --lib`
 LOADLIBS += `aida-config --lib`
# LOADLIBS += `aida-config --lib` -ldl
endif

