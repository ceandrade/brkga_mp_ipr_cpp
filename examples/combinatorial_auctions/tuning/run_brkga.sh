#!/bin/bash

# R parameters
CONFIG_ID="$1"
INSTANCE_ID="$2"

EXE="./brkga-ca"
CONFIG_FILE="brkga_mp_ipr.conf"  # Not used in the tuning
SEED="${3}"
STOP_RULE="Iterations 1000"
DECODE_APPROACH="Greedy"
INIT_CONFIG="Random 0 0 0 0"
MAX_TIME="600"
PRICING_SCHEME="FirstPrice"
INSTANCE="${4}"
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

