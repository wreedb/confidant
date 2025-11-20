#!/usr/bin/env bash
# vim:ft=bash

export CC=clang
export CXX=clang++
export CPP=clang-cpp

cflagsarr=(
    -fstack-clash-protection
    -fcf-protection=full
    -fstack-protector-strong
    -fdiagnostics-color
    -fPIC
    -pipe
    -march=x86-64
    -mtune=generic
    -O2
    -Wp,-D_FORTIFY_SOURCE=2
)

export CFLAGS="${cflagsarr[@]}"
export CXXFLAGS="${cflagsarr[@]} -std=c++23 -stdlib=libc++"