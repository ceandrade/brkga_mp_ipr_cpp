###############################################################################
# (c) Copyright 2019-2022, Carlos Eduardo de Andrade. All Rights Reserved.
#
# This code is released under LICENSE.md.
#
# Created on:  Dec 28, 2018 by ceandrade
# Last update: Jan 09, 2019 by ceandrade
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

#!/bin/bash

# R parameters
CONFIG_ID="$1"
INSTANCE_ID="$2"
SEED="${3}"
INSTANCE="${4}"

EXE="./brkga-ca"
CONFIG_FILE="brkga_mp_ipr.conf"  # Not used in the tuning
STOP_RULE="Iterations 1000"
DECODE_APPROACH="Greedy"
INIT_CONFIG="Random 0 0 0 0"
MAX_TIME="600"
PRICING_SCHEME="FirstPrice"
INSTANCE_LP="/dev/null"
POPSIZE="${5}"
PE="${6}"
PM="${7}"
ELITE_PARENTS="${8}"
TOTAL_PARENTS="${9}"
BIAS="${10}"
INDPOP="${11}"
PR_DISTANCE="${12}"
PR_TYPE="${13}"
PR_SELECTION="${14}"
ALPHA_BLOCK_SIZE="${15}"
PR_PERCENTAGE="${16}"
INTERVALEXCHANGE="${17}"
ELITEXCHANGE="${18}"
RESET="${19}"

${EXE} \
    ${CONFIG_FILE} \
    ${SEED} \
    ${STOP_RULE} \
    ${DECODE_APPROACH} \
    ${INIT_CONFIG} \
    ${MAX_TIME} \
    ${PRICING_SCHEME} \
    ${INSTANCE} \
    ${INSTANCE_LP} \
    ${POPSIZE} \
    ${PE} \
    ${PM} \
    ${ELITE_PARENTS} \
    ${TOTAL_PARENTS} \
    ${BIAS} \
    ${INDPOP} \
    ${PR_DISTANCE} \
    ${PR_TYPE} \
    ${PR_SELECTION} \
    ${ALPHA_BLOCK_SIZE} \
    ${PR_PERCENTAGE} \
    ${INTERVALEXCHANGE} \
    ${ELITEXCHANGE} \
    ${RESET} \
    | tail -n 1

