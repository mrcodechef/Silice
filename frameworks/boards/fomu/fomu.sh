#!/bin/bash

# credits: rob-ng15 -- see also https://github.com/rob-ng15/Silice-Playground

case "$(uname -s)" in
MINGW*|CYGWIN*) 
SILICE_DIR=`cygpath $SILICE_DIR`
BUILD_DIR=`cygpath $BUILD_DIR`
FRAMEWORK_FILE=`cygpath $FRAMEWORK_FILE`
BOARD_DIR=`cygpath $BOARD_DIR`
;;
*)
esac

echo "build script: SILICE_DIR     = $SILICE_DIR"
echo "build script: BUILD_DIR      = $BUILD_DIR"
echo "build script: BOARD_DIR      = $BOARD_DIR"
echo "build script: FRAMEWORK_FILE = $FRAMEWORK_FILE"

export PATH=$PATH:$SILICE_DIR/../tools/fpga-binutils/mingw64/bin/:$SILICE_DIR
export PYTHONHOME=/mingw64/bin
export PYTHONPATH=/mingw64/lib/python3.8/
export QT_QPA_PLATFORM_PLUGIN_PATH=/mingw64/share/qt5/plugins

cd $BUILD_DIR

rm build*

silice -f $FRAMEWORK_FILE $1 -o build.v "${@:2}"

yosys -D HACKER=1 -p 'synth_ice40 -top top -json build.json' build.v

nextpnr-ice40 --up5k --package uwg30 --opt-timing --pcf $BOARD_DIR/fomu-hacker.pcf --json build.json --asc build.asc

icepack build.asc build.bit
icetime -d up5k -mtr build.rpt build.asc

cp build.bit build.dfu
dfu-suffix -v 1209 -p 70b1 -a build.dfu
dfu-util -D build.dfu
