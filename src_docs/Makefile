################################################################################
# (c) Copyright 2019-2023, Carlos Eduardo de Andrade.
# All Rights Reserved.
#
#  Created on : Mar 08, 2019 by andrade
#  Last update: Sep 22, 2023 by andrade
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
################################################################################

################################################################################
# Required tools
################################################################################

#REQUIRED_TOOLS := doxygen doxyrest sphinx-build

#$(foreach tool,$(REQUIRED_TOOLS),\
    #$(if $(shell command -v $(tool) 2> /dev/null),,\
        #$(error `$(tool)` not found in PATH. Please install `$(tool)`)\
	 #)\
#)

################################################################################
## Build Rules
################################################################################

.PHONY: all clean

all: doxygen doxyrest sphinx
	echo

doxygen:
	doxygen Doxyfile

doxyrest:
	DYLD_LIBRARY_PATH=$DYLD_LIBRARY_PATH:/opt/local/lib \
	./doxyrest-2.1.3/bin/doxyrest -c doxyrest-config.lua
	# cp build/rst_doxyrest/* src/rst/.

sphinx:
	rm -rf build/rst_tmp
	rm -rf build/html_sphinx
	cp -a src/rst build/rst_tmp
	cp -a src/assets build/.
	sphinx-build -b html build/rst_tmp build/html_sphinx
	touch build/html_sphinx/.nojekyll
	rm -rf ../docs
	cp -a build/html_sphinx ../docs
	cp -a src/assets/logo*png ../docs/_static

clean:
	rm -rf build

