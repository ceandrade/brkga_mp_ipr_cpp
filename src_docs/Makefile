###############################################################################
# (c) Copyright 2019, Carlos Eduardo de Andrade.
# All Rights Reserved.
#
#  Created on : Mar 08, 2019 by andrade
#  Last update: Oct 25, 2019 by andrade
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
# Required tools
###############################################################################

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

all: sphinx
	echo

doxygen:
	doxygen Doxyfile

doxyrest:
	doxyrest -c doxyrest-config.lua

sphinx:
	mkdir -p build
	cp -a src/rst build/.
	cp -a src/assets build/.
	sphinx-build -b html build/rst build/html_sphinx
	touch  build/html_sphinx/.nojekyll
	rm -rf ../docs
	cp -a  build/html_sphinx ../docs
	cp -a src/assets/logo*png ../docs/_static

clean:
	rm -rf build

