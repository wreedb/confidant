#!/usr/bin/env fish
# vim:ft=fish

set -gx CC clang
set -gx CXX clang++
set -gx CPP clang-cpp

set -gx CFLAGS -fstack-clash-protection \
    -fcf-protection=full \
    -fstack-protector-strong \
    -fdiagnostics-color \
    -fPIC \
    -pipe \
    -march=x86-64 \
    -mtune=generic \
    -O2 \
    -Wp,-D_FORTIFY_SOURCE=2

set -gx CXXFLAGS $CFLAGS -std=c++23 -stdlib=libc++