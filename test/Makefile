#OPT = opt
USER_FLAGS += -std=c++14
ifneq ($(OPT), opt)
	USER_FLAGS += -ggdb3 -fexceptions -fno-omit-frame-pointer \
		-fno-optimize-sibling-calls -fno-inline
else
	USER_FLAGS += -O3 -fomit-frame-pointer -ftracer -funroll-loops \
		-fpeel-loops -fprefetch-loop-arrays
endif

USER_FLAGS += -m64 -pthread -fexceptions

USER_FLAGS += -Wall -Wextra -Wcast-align -Wcast-qual -Wdisabled-optimization \
	-Wformat=2 -Winit-self -Wmissing-format-attribute -Wshadow \
	-Wpointer-arith -Wredundant-decls -Wstrict-aliasing=2 \
	-Wfloat-equal -Weffc++

# Avoid problems in Mac OS X on GCC <= 5.2
USER_FLAGS += -mno-avx

USER_FLAGS += -Winline
USER_FLAGS += -Wpacked

#USER_FLAGS += -Wpadded
#USER_FLAGS += -Wconversion
#USER_FLAGS += -Wunsafe-loop-optimizations # doesn't work very well with C++11 foreach

CXX = g++
#CXX = clang++ -fopenmp


COMMON_OBJS = decoders.o

.PHONY: all
.SUFFIXES: .cpp .o

.cpp.o:
	@echo "--> Compiling $<..."
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr -c $< -o $@
	@echo

test-brkga: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_brkga_mp_ipr.cpp -o test-brkga
	./test-brkga 5 1 2700001

test-ipr: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_ipr.cpp -o test-ipr
	./test-ipr 5 1 2700001

test-read: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_readfile.cpp -o test-read
	./test-read 100 brkga_mp_ipr.conf 2700001

test-params: clean $(COMMON_OBJS)
	$(CXX) $(USER_FLAGS) -I../brkga_mp_ipr $(COMMON_OBJS) test_params.cpp -o test-brkga
	./test-params 100 brkga_mp_ipr.conf 2700001

check:
	cppcheck --enable=all ../brkga_mp_ipr/brkga_mp_ipr.hpp

clean:
	rm -rf $(COMMON_OBJS) test-*  *.dSYM

docclean:
	@echo "--> Cleaning doc..."
	rm -rf doc
