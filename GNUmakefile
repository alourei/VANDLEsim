# $Id: GNUmakefile 12.07.2016 A.Fijalkowska $
# --------------------------------------------------------------
# GNUmakefile for VANDLE simmulation proj.
# --------------------------------------------------------------

name := VANDLESim
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/architecture.gmk

include $(G4INSTALL)/config/binmake.gmk
