#!/bin/sh -e

CFLAGS_WARN="-Wall -Wextra -Wno-unused-parameter -Wno-unused-variable -Wno-sign-compare -Wno-pointer-sign -Wno-missing-field-initializers"
CFLAGS="-g -O0"

re2c -s -o sjson.gen.c sjson.c

# coverage test

rm -f *.gcno *.gcda
gcc --coverage $CFLAGS -I. -o sjson-test test.c sjson.gen.c `pkg-config --libs --cflags glib-2.0`
./sjson-test

rm -rf coverage coverage.info
lcov --capture --directory . --output-file coverage.info
genhtml coverage.info --output-directory coverage

# leaks test

export G_SLICE=always-malloc G_DEBUG=gc-friendly
FLAGS=--gen-suppressions=all
FLAGS=

valgrind \
  --leak-check=full \
  --leak-resolution=high \
  --show-reachable=yes \
  --log-file=leak.log \
  --suppressions=leak.supp \
  $FLAGS \
  ./sjson-test

rm -f *.gcno *.gcda coverage.info
