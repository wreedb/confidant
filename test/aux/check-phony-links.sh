#!/usr/bin/env bash

phonydirs=(
    foo
    bar
    fizz
    fuzz
    buzz
    lorem
);

for d in ${phonydirs[@]}; do
    command readlink ~/.config/${d} || :
done