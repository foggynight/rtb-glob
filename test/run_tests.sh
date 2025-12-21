#!/bin/sh

set -xe

gcc -o test test.c -Wall -Wextra -Wpedantic
./test
