#!/bin/sh -e

ROOTDIR=$(realpath "$(dirname "$0")/..")
SRCDIR="$ROOTDIR/src"
BUILDDIR="$ROOTDIR/build"

# Расположите свою команду на следующей строке
gcc build/main.o -o build/main
