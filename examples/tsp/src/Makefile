###############################################################################
# (c) Copyright 2019, Carlos Eduardo de Andrade.
# All Rights Reserved.
#
#  Created on : Mar 05, 2019 by andrade
#  Last update: May 03, 2019 by andrade
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

###############################################################################
# Build options
###############################################################################

# Include dir
INCLUDES = \
	-I. \
	-I../../../brkga_mp_ipr

# Objects
OBJS = \
	./tsp/tsp_instance.o \
	./heuristics/greedy_tour.o \
	./decoders/tsp_decoder.o \
	./decoders/tsp_decoder_pre_allocating.o

BRKGA_MAIN = ./brkga/main_brkga.o
BRKGA_EXE = brkga-scheduler

MAIN_COMPLETE_OBJ = main_complete.o
MAIN_COMPLETE_EXE = main_complete

MAIN_MINIMAL_OBJ = main_minimal.o
MAIN_MINIMAL_EXE = main_minimal

###############################################################################
# Compiler, linker, and flags
###############################################################################

# C++ Compiler command and flags
CXX = g++
# CXX = clang++
# CXX = /opt/local/bin/clang++-mp-7.0

# Define the usage of C++14. This code compiles only in modern compilers,
# for instance, gcc >= 5.2
USER_FLAGS += -std=c++14

# Compiler flags for debugging
ifneq ($(OPT), opt)
	USER_FLAGS += -ggdb3 -fexceptions -fno-omit-frame-pointer \
		-fno-optimize-sibling-calls -fno-inline
else
	USER_FLAGS += -O3 -fomit-frame-pointer -funroll-loops
	ifeq ($(CXX), g++)
		USER_FLAGS += -ftracer -fpeel-loops -fprefetch-loop-arrays
	endif
endif

# Include parallel stuff
USER_FLAGS += -pthread -fopenmp

# Warning flags
USER_FLAGS += -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization \
	-Wformat=2 -Winit-self -Wmissing-format-attribute -Wshadow \
	-Wpointer-arith -Wredundant-decls -Wstrict-aliasing=2 \
	-Wfloat-equal -Weffc++

ifeq ($(CXX), g++)
	USER_FLAGS += -Wunsafe-loop-optimizations
endif

# Paranoid mode!!!
# Used to analyze format of structures
# USER_FLAGS += -Wpacked -Wpadded
# USER_FLAGS += -Wconversion

CXXFLAGS = $(USER_FLAGS)

###############################################################################
# Build Rules
###############################################################################

.PHONY: all clean
.SUFFIXES: .cpp .o

all: main_minimal main_complete

main_minimal: $(OBJS) $(MAIN_MINIMAL_OBJ)
	@echo "--> Linking objects... "
	$(CXX) $(CXXFLAGS) $(OBJS) $(MAIN_MINIMAL_OBJ) -o $(MAIN_MINIMAL_EXE)
	@echo

main_complete: $(OBJS) $(MAIN_COMPLETE_OBJ)
	@echo "--> Linking objects... "
	$(CXX) $(CXXFLAGS) $(OBJS) $(MAIN_COMPLETE_OBJ) -o $(MAIN_COMPLETE_EXE)
	@echo

.cpp.o:
	@echo "--> Compiling $<..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(USER_DEFINES) -c $< -o $@
	@echo

clean:
	@echo "--> Cleaning compiled..."
	rm -rf $(OBJS) $(MAIN_MINIMAL_OBJ) $(MAIN_COMPLETE_OBJ) $(MAIN_MINIMAL_EXE) $(MAIN_COMPLETE_EXE)
	rm -rf *o
	rm -rf Debug
	rm -rf *.dSYM
