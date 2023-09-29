###############################################################################
# Makefile: building instructions using GNU Make utilities.
#
# (c) Copyright 2019-2023, Carlos Eduardo de Andrade.
# All Rights Reserved.
#
# Created on : Mar 05, 2019 by ceandrade
# Last update: Sep 14, 2023 by ceandrade
#
# This code is released under BRKGA-MP-IPR License:
# https://github.com/ceandrade/brkga_mp_ipr_cpp/blob/master/LICENSE.md
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

OPT = opt
USER_FLAGS += -std=c++20 -fopenmp -DBRKGA_MULTIPLE_INCLUSIONS

# Compiler flags for debugging
ifeq ($(OPT), opt)
	USER_FLAGS += -O3 -fomit-frame-pointer -funroll-loops
	ifeq ($(CXX), g++)
		USER_FLAGS += -ftracer -fpeel-loops -fprefetch-loop-arrays
	endif
else
	USER_FLAGS += -ggdb3 -fexceptions -fno-omit-frame-pointer \
		-fno-optimize-sibling-calls -fno-inline
endif

# Flags for debugging
USER_FLAGS += -pthread -Wall -Wextra -Wcast-align -Wcast-qual \
	-Wdisabled-optimization -Wformat=2 -Winit-self -Wmissing-format-attribute \
	-Wshadow -Wpointer-arith -Wredundant-decls -Wstrict-aliasing=2 \
	-Wfloat-equal -Weffc++

USER_FLAGS += -Winline
USER_FLAGS += -Wpacked
#USER_FLAGS += -Wpadded
#USER_FLAGS += -Wconversion
#USER_FLAGS += -Wunsafe-loop-optimizations # doesn't work very well with C++11 foreach

# For inlining debugging
# USER_FLAGS += -fopt-info-inline

# Flags for covering tests.
#USER_FLAGS += -fPIC -fprofile-arcs -ftest-coverage

CXX = g++
#CXX = clang++
#CXX = clang++-10

COMMON_OBJS = decoders.o

.PHONY: all
.SUFFIXES: .cpp .o

.cpp.o:
	@echo "--> Compiling $<..."
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr -c $< -o $@
	@echo

test_params: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_params.cpp -o $@
	./test-params config_full.conf

test_custom_bias_function: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_custom_bias_function.cpp -o test-custom_bias_function

test_brkga_mp_ipr: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_brkga_mp_ipr.cpp -o test-brkga
	./test-brkga 5 1 2700001

test_ipr: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_ipr.cpp -o test-ipr
	./test-ipr 5 1 2700001

test_readfile: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_readfile.cpp -o test-read
	./test-read 100 brkga_mp_ipr.conf 2700001

test_run: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) $@.cpp -o $@
	./$@ 100 2700001

test_speed: clean $(COMMON_OBJS)
	# $(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_speed.cpp -o test_new_version
	$(CXX) $(USER_FLAGS) -Ibrkga_mp_ipr_old $(COMMON_OBJS) -DOLD_VERSION test_speed.cpp -o test_old_version

test_multiple_inclusions: 
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr -c module_a.cpp -o module_a.o
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr -c module_b.cpp -o module_b.o
	$(CXX) $(USER_FLAGS)  module_a.o module_b.o -o test


check:
	cppcheck --enable=all --suppress=missingIncludeSystem ../brkga_mp_ipr/brkga_mp_ipr.hpp

coverage:
	gcov test_run.cpp test_run-test_run*

lcov-report: coverage
	mkdir lcov-report
	lcov --capture --directory . --output-file lcov-report/coverage.info
	genhtml lcov-report/coverage.info --output-directory lcov-report

clean:
	rm -rf $(COMMON_OBJS) test-* *.dSYM *.gcno *.gcda *.gcov

docclean:
	@echo "--> Cleaning doc..."
	rm -rf doc
