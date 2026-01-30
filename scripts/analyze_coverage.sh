#!/bin/sh

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd $SCRIPT_DIR/..

mkdir -p ./build/code-coverage/coverage-data
lcov \
    --directory ./build/code-coverage/ \
    --capture \
    --ignore-errors mismatch \
    --output-file ./build/code-coverage/coverage-data/coverage.raw.info

lcov \
    --ignore-errors unused \
    --remove ./build/code-coverage/coverage-data/coverage.raw.info "*/build/*" "/usr/*" "*/src/app/*" "*/vcpkg_installed/*" "*/src/test/*" \
    --output-file ./build/code-coverage/coverage-data/coverage.filtered.info
