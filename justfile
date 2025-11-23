# SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
#
# SPDX-License-Identifier: GPL-3.0-or-later

version := `cat .version`

[group("build")]
build-static: clean-build
    meson setup .build --prefix=/ --buildtype=release -D static=true -D static-stdlib=true --wrap-mode=forcefallback
    meson compile -C .build

[group("build")]
build: clean-build
    meson setup .build --prefix=/ --buildtype=release --strip
    meson compile -C .build
    
[group("clean")]
clean-build:
    -rm -rf .build

[group("clean")]
clean-cache:
    -rm -rf .cache

[group("clean")]
distclean: clean-build clean-cache clean-flake

[group("flake")]
build-flake:
    nix build

[group("flake")]
update-flake:
    nix flake update
    
[group("clean")]
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