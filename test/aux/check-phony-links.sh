#!/usr/bin/env bash

phonys=(
    foo/foo.conf
    bar
    fizz/fizz.cfg
    fuzz
    buzz/buzz.yaml
    lorem/ipsum
);

for d in ${phonys[@]}; do
    if [ -L "${HOME}/.config/${d}" ]; then
        echo "~/.config/${d}: pass"
    else
        echo "~/.config/${d}: fail"
    fi
done