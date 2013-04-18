#!/bin/sh -e

CFLAGS_WARN="-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-sign-compare -Wno-pointer-sign -Wno-missing-field-initializers"
CFLAGS="-mtune=core-avx-i -march=core-avx-i -O3"

re2c -s -o sjson.gen.c sjson.c

rm -f *.gcda
gcc $CFLAGS -fprofile-generate -I. -o sjson-bench bench.c sjson.gen.c `pkg-config --libs --cflags glib-2.0 json-glib-1.0`
./sjson-bench
gcc $CFLAGS -fprofile-use -I. -o sjson-bench bench.c sjson.gen.c `pkg-config --libs --cflags glib-2.0 json-glib-1.0`
./sjson-bench
rm -f *.gcda
