#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [ -d "$SCRIPT_DIR/build/" ]; then 
    rm -rf $SCRIPT_DIR/build/
fi

cd $SCRIPT_DIR
cmake -S . -B ./build/
cd ./build/
make -j$(nproc) -l$(nproc)
rm -rf cpr-* libssh2-* openssl-* nghttp2-* ftxui-* ftxui_ext-*
cp -r ../scripts/ .
./installer_part1