# SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
#
# SPDX-License-Identifier: GPL-3.0-or-later

version-file := ".version"
version := `cat .version`

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