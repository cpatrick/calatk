# Copyright (C) 2004, International Business Machines and others.
# All Rights Reserved.
# This file is distributed under the Eclipse Public License.

## $Id: Makefile.am 1861 2010-12-21 21:34:47Z andreasw $

# Authors:  Carl Laird, Andreas Waechter     IBM    2004-09-02

AUTOMAKE_OPTIONS = foreign

noinst_LIBRARIES = libcompositenlp.a
bin_PROGRAMS = composite_ampl_ipopt

libcompositenlp_a_SOURCES = \
	CompositeNLP.cpp CompositeNLP.hpp

composite_ampl_ipopt_SOURCES = \
	comp_ampl_ipopt.cpp

IPLIBS = \
	../../Interfaces/libinterfaces.la \
	../../Algorithm/libipoptalg.la \
	../../Algorithm/LinearSolvers/liblinsolvers.la \
	../../LinAlg/TMatrices/libtmatrices.la \
	../../LinAlg/liblinalg.la \
	../../Common/libcommon.la \
	../AmplSolver/libipoptamplinterface.la

composite_ampl_ipopt_LDADD = libcompositenlp.la $(IPLIBS) $(ASL_LIBS) $(IPOPTLIB_LIBS)
composite_ampl_ipopt_DEPENDENCIES = libcompositenlp.la $(IPLIBS) $(ASL_DEPENDENCIES) $(IPOPTLIB_DEPENDENCIES)

CXXFLAGS += -I$(srcdir)/../../Common \
	-I$(srcdir)/../../LinAlg \
	-I$(srcdir)/../../LinAlg/TMatrices \
	-I$(srcdir)/../../Algorithm \
	-I$(srcdir)/../../Interfaces \
	-I$(srcdir)/../AmplSolver

#CXXFLAGS += -I$(srcdir)/../../Apps/AmplSolver \
#	    -I$(srcdir)/../../OTHERS/ASL/ \
#	    -I$(srcdir)/../../OTHERS/ASL/solvers
CXXFLAGS += $(AMPL_CPPFLAGS)

# Astyle stuff

ASTYLE_FILES = \
	CompositeNLP.cppbak CompositeNLP.hppbak \
	comp_ampl_ipopt.cppbak

ASTYLE = @ASTYLE@
ASTYLEFLAGS = @ASTYLEFLAGS@

DISTCLEANFILES = $(ASTYLE_FILES)

SUFFIXES = .cppbak .hppbak

astyle: $(ASTYLE_FILES)

.hpp.hppbak:
	mv $< $@
	$(ASTYLE) $(ASTYLEFLAGS) < $@ > $<
	touch $@

.cpp.cppbak:
	mv $< $@
	$(ASTYLE) $(ASTYLEFLAGS) < $@ > $<
	touch $@
