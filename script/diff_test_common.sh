#!/bin/bash
run_diff_test() {
    local MAKE_TARGET="$1"
    local MAKEFILE_PATH="$2"

    local SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    local ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

    if [ -z "${MAKEFILE_PATH}" ]; then
        MAKEFILE_PATH="${ROOT}/Makefile"
    fi

    local MAKE_DIR="$(dirname "${MAKEFILE_PATH}")"
    local MAKE_FILE="$(basename "${MAKEFILE_PATH}")"

    local BIN_DIR="${ROOT}/bin"
    local LOG_DIR="${ROOT}/logs"
    local NAME=containers
    local SEED=$(( $(date +%s) % 100000 ))
    local TS="$(date +'%y%m%d_%H%M%S')"

    mkdir -p "${LOG_DIR}"

    local C0="\033[0m"; local C1="\033[96m"; local CG="\033[92m"
    local CR="\033[91m"; local CY="\033[93m"; local Cx="\033[90m"

    echo -e "${C1}==========================================${C0}"
    echo -e "${C1} Running DIFF for target: ${MAKE_TARGET}${C0}"
    echo -e "${C1}------------------------------------------${C0}"
    echo -e "${Cx}Seed: ${SEED}${C0}"
    echo -e "${Cx}Makefile: ${MAKEFILE_PATH}${C0}"
    echo -e "${Cx}Logs dir: ${LOG_DIR}/${C0}"
    echo -e "${Cx}Time: ${TS}${C0}"
    echo -e "${C1}==========================================${C0}"

    echo -e "${CY}→ Cleaning old builds...${C0}"
    make -C "${MAKE_DIR}" -f "${MAKE_FILE}" fclean >/dev/null

    build_one () {
        local NS="$1"
        local EXTRA=""
        if [[ "${MAKE_TARGET}" == *_utils_* ]]; then
            EXTRA="CXXSTD=c++11"
            echo -e "${CY}→ Using CXXSTD=c++11 for utils build${C0}"
        fi
        echo -e "${CY}→ Building ${NS}: make ${MAKE_TARGET}${C0}"
        make -C "${MAKE_DIR}" -f "${MAKE_FILE}" ${MAKE_TARGET} NAMESPACE=${NS} ${EXTRA} >/dev/null || {
            echo -e "${CR}[ERROR] build failed (${NS}, ${MAKE_TARGET})${C0}"
            exit 1
        }
    }

    build_one ft
    build_one std

    local TEST_NAME="${MAKE_TARGET}"
    local FT_BIN="${BIN_DIR}/${NAME}_ft_${TEST_NAME}"
    local STD_BIN="${BIN_DIR}/${NAME}_std_${TEST_NAME}"

    if [ ! -x "${FT_BIN}" ]; then
        echo -e "${CR}[ERROR] Missing ft binary: ${FT_BIN}${C0}"; exit 1; fi
    if [ ! -x "${STD_BIN}" ]; then
        echo -e "${CR}[ERROR] Missing std binary: ${STD_BIN}${C0}"; exit 1; fi

    local FT_LOG="${LOG_DIR}/${TS}_${TEST_NAME}_ft.log"
    local STD_LOG="${LOG_DIR}/${TS}_${TEST_NAME}_std.log"
    local DIFF_LOG="${LOG_DIR}/${TS}_${TEST_NAME}_diff.log"

    echo -e "${CY}→ Running tests...${C0}"
    echo -e "${Cx}FT : ${FT_BIN}${C0}"
    echo -e "${Cx}STD: ${STD_BIN}${C0}"

    if [[ "${MAKE_TARGET}" == stress_* ]]; then
        "${FT_BIN}"  "${SEED}" > "${FT_LOG}" 2>&1
        "${STD_BIN}" "${SEED}" > "${STD_LOG}" 2>&1
    else
        "${FT_BIN}"  > "${FT_LOG}" 2>&1
        "${STD_BIN}" > "${STD_LOG}" 2>&1
    fi

    echo -e "${CY}→ Comparing outputs...${C0}"
    if diff -u "${STD_LOG}" "${FT_LOG}" > "${DIFF_LOG}"; then
        echo -e "${CG}[PASS]${C0} Outputs identical for ${TEST_NAME}"
    else
        echo -e "${CR}[FAIL]${C0} Differences found for ${TEST_NAME}"
        echo -e "${Cx}----- DIFF START -----${C0}"
        head -n 30 "${DIFF_LOG}"
        echo -e "${Cx}----- (truncated) -----${C0}"
    fi

    echo -e "${C1}------------------------------------------${C0}"
    echo -e "${Cx}FT log : ${FT_LOG}${C0}"
    echo -e "${Cx}STD log: ${STD_LOG}${C0}"
    echo -e "${Cx}DIFF   : ${DIFF_LOG}${C0}"
    echo -e "${C1}==========================================${C0}"
}
