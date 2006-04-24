# ---------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
# ---------------------------------------------------------------------------
# automake input file
#
noinst_PROGRAMS    = \
	ExpApxMain \
	ExpApxSeqMain \
	ExpApxFor \
	ExpApxRev \
	ExpApxAD
#
noinst_DATA        = \
	ExpApxMain.out \
	ExpApxSeq.out \
	ExpApxFor.out \
	ExpApxRev.out \
	ExpApxAD.out
#
if CppAD_BOOSTVECTOR
AM_CPPFLAGS       = -I$(BOOST_DIR) \
                    -I. -I$(top_builddir)
else
AM_CPPFLAGS       = -I. -I$(top_builddir)
endif
#
AM_CXXFLAGS = -g $(CPP_ERROR_WARN)
#
EXTRA_DIST = ExpApx.hpp
#
ExpApxMain_SOURCES = ExpApxMain.cpp 
ExpApxSeqMain_SOURCES = ExpApxSeqMain.cpp ExpApxSeq.cpp
ExpApxFor_SOURCES = ExpApxFor.cpp ExpApxSeq.cpp
ExpApxRev_SOURCES = ExpApxRev.cpp ExpApxSeq.cpp
ExpApxAD_SOURCES = ExpApxAD.cpp 
#
ExpApxMain.out: ExpApxMain$(EXEEXT)
	./ExpApxMain > ExpApxMain.out
#
ExpApxSeq.out: ExpApxSeqMain$(EXEEXT)
	./ExpApxSeqMain > ExpApxSeq.out
#
ExpApxFor.out: ExpApxFor$(EXEEXT)
	./ExpApxFor > ExpApxFor.out
#
ExpApxRev.out: ExpApxRev$(EXEEXT)
	./ExpApxRev > ExpApxRev.out
#
ExpApxAD.out: ExpApxAD$(EXEEXT)
	./ExpApxAD > ExpApxAD.out
