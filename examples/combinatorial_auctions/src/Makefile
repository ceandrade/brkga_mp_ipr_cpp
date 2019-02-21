###############################################################################
# Author: Carlos Eduardo de Andrade <ce.andrade@gmail.com>
#
# (c) Copyright 2015
#     Institute of Computing, University of Campinas.
#     All Rights Reserved.
#
#  Created on : Jan 13, 2015 by andrade
#  Last update: Jan 13, 2015 by andrade
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
###############################################################################

###############################################################################
# User Defines
###############################################################################

# Optimization switch
OPT = opt

# Set debug mode
#USER_DEFINES += -DDEBUG
#USER_DEFINES += -DFULLDEBUG 

# Set tuning mode
#USER_DEFINES += -DTUNING

###############################################################################
# Build options
###############################################################################

EXE = brkga-ca

###############################################################################
# The user source files
###############################################################################

# Include dir
USER_INCDIRS = \
	. \
	./include \
	./brkga_mp_ipr

# Source directories
SRC_DIRS = \
	./problem_defs/ \
	./decoders/

# Object files
OBJS = \
	./problem_defs/combinatorial_auction.o \
	./decoders/combinatorial_auction_decoder.o \
	./main_brkga_mp_ipr.o

###############################################################################
# Compiler flags
###############################################################################

ifeq ($(shell uname -s),Darwin)
    SYSTEM = macosx
else
    SYSTEM = linux
endif

# Define the usage of C++14. This code compiles only in modern compilers,
# for instance, gcc >= 5.2
USER_FLAGS += -std=c++14

# Compiler flags for debugging
ifneq ($(OPT), opt)
	USER_FLAGS += -ggdb3 -fexceptions -fno-optimize-sibling-calls -fno-inline
	# -fsanitize=address
	# -fsanitize=undefined
else
	USER_FLAGS += -O3 -m64 \
		-fomit-frame-pointer -ftracer \
		-funroll-loops -fpeel-loops -fprefetch-loop-arrays
endif

# GCC <= 5.2 has some problem in auto detect some Intel chips.
# This avoid use avx extensions. If your compiler can detect correctly,
# comment the following line.
#
ifeq ($(SYSTEM), macosx)
	USER_FLAGS += -mno-avx
endif

# Flags
USER_FLAGS += -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization \
				-Wformat=2 -Winit-self -Wmissing-format-attribute -Wshadow \
				-Wpointer-arith -Wredundant-decls -Wstrict-aliasing=2 \
				-Wfloat-equal -Weffc++

# Paranoid!!!
#USER_FLAGS += -Wconversion
#USER_FLAGS += -Wunsafe-loop-optimizations # doesn't work very well with C++11 foreach

USER_FLAGS += -pthread -fopenmp 

###############################################################################
# Lib and include definitions
###############################################################################

ifeq ($(SYSTEM), macosx)
	INCLUDE_PATH = /opt/local
	USER_LIBDIRS = /opt/local/lib 
else
	#INCLUDE_PATH = <path to your include dir (usually /usr/include)>
	#USER_LIBDIRS = <path to your lib dir  (usually /usr/lib) >
endif

USER_INCDIRS += \
	$(INCLUDE_PATH)/include \
	$(INCLUDE_PATH)/include/boost
	
INCLUDES = `for i in $(USER_INCDIRS); do echo $$i | sed -e 's/^/-I/'; done`	
LIBDIRS = `for i in $(USER_LIBDIRS); do echo $$i | sed -e 's/^/-L/'; done`

#Boost libraries
USER_DEFINES+= -DBOOST_FILESYSTEM_NO_DEPRECATED

ifeq ($(SYSTEM), macosx)
    BOOST_LIBS += boost_system-mt boost_chrono-mt boost_timer-mt boost_filesystem-mt
else
    BOOST_LIBS += boost_system boost_chrono boost_timer boost_filesystem
endif

LIBS = -lm 
LIBS += `for i in $(BOOST_LIBS); do echo $$i | sed -e 's/^/-l/'; done`

###############################################################################
# Compiler and linker defs
###############################################################################

# C Compiler command and flags
CXX = g++
CXXFLAGS = $(USER_FLAGS)

# Linker
LD = ld

# Lib maker commands
AR = ar
ARFLAGS	= rv
RANLIB = ranlib

# Other includes
RM = rm
SHELL = /bin/bash

# Necessary Include dirs
# Put -I in front of dirs
INCLUDES = `for i in $(USER_INCDIRS); do echo $$i | sed -e 's/^/-I/'; done`

# Necessary library dirs
# Put -L in front of dirs
LIBDIRS = `for i in $(USER_LIBDIRS); do echo $$i | sed -e 's/^/-L/'; done`

###############################################################################
# Build Rules
###############################################################################

.PHONY: all strip clean doc docclean depclean
.SUFFIXES: .cpp .o

all: $(EXE)

$(EXE): $(OBJS)
	@echo "--> Linking objects... "
	$(CXX) $(CXXFLAGS) $(LINKER_FLAGS) $(OBJS) -o $@ $(LIBDIRS) $(LIBS) 
	@echo

.cpp.o:
	@echo "--> Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(USER_DEFINES) -c $< -o $@
	@echo

strip: $(EXE)
	@echo "-->Stripping $<..."
	strip $<
	@echo

doc:
	doxygen Doxyfile

clean:
	@echo "--> Cleaning compiled..."
	rm -rf $(EXE) $(OBJS) $(CLIQUER_OBJS)
	
depclean: docclean clean
	rm -rf `for i in $(SRC_DIRS); do echo $$i*~; done` 
	rm -rf Debug
	
docclean:
	@echo "--> Cleaning doc..."
	rm -rf doc
