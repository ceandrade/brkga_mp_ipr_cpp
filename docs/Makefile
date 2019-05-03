###############################################################################
# (c) Copyright 2019, Carlos Eduardo de Andrade.
# All Rights Reserved.
#
#  Created on : Mar 08, 2019 by andrade
#  Last update: Mar 08, 2019 by andrade
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
# Doxyrest paths
###############################################################################

DOXYREST_FRAME_DIR=~/temp/doxyrest-1.1.1-a-mac/share/doxyrest/frame

###############################################################################
# Required tools
###############################################################################

REQUIRED_TOOLS := doxygen doxyrest sphinx-build

$(foreach tool,$(REQUIRED_TOOLS),\
    $(if $(shell command -v $(tool) 2> /dev/null),,\
  	  $(error `$(tool)` not found in PATH. Please install `$(tool)`)\
	 )\
)

################################################################################
## Build Rules
################################################################################

.PHONY: all clean

all: doxygen doxyrest sphinx
	echo

doxygen:
	doxygen Doxyfile

doxyrest:
	doxyrest build/xml/index.xml -o build/tmp_rst/index.rst \
		-F $(DOXYREST_FRAME_DIR) -f c_index.rst.in

sphinx:
	sphinx-build -c . -b html build/tmp_rst build/html_sphinx

clean:
	rm -rf build

