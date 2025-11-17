# SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
#
# SPDX-License-Identifier: GPL-3.0-or-later

version := `cat .version`

[group("build")]
build: clean-build
    meson setup .build --prefix=/usr --buildtype=release --strip -D static-stdlib=true
    meson compile -C .build
    
[group("clean")]
clean-build:
    -rm -rf .build/

[group("flake")]
build-flake:
    nix build

[group("flake")]
clean-flake:
    -rm result

[group("git")]
commit:
    convco commit -i

[group("maintainence")]
bump amt:
    @semver -i {{amt}} {{version}}

[group("maintainence")]
changelog:
    @git cliff

completions:
    complgen misc/completions/confidant.usage --bash misc/completions/bash/confidant.bash
    complgen misc/completions/confidant.usage --zsh  misc/completions/zsh/_confidant