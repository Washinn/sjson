#!/bin/sh -e

CFLAGS_WARN="-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-sign-compare -Wno-pointer-sign -Wno-missing-field-initializers"
CFLAGS="-g -O0"

#CFLAGS="$CFLAGS -fplugin=./gcc/plugin.so"

re2c -s -o sjson.gen.c sjson.c

gcc $CFLAGS -I. -o sjson-test test.c sjson.gen.c `pkg-config --libs --cflags glib-2.0`
./sjson-test
