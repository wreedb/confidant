<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: GPL-3.0-or-later
-->

Requirements
==================

**Confidant** uses the [Meson](https://mesonbuild.com) build system, and 
therefore also requires [Ninja](https://ninja-build.org).

A compiler with support for at least C++23 is also needed, such as:
- [GCC](https://gcc.gnu.org) >= version 14,
- [Clang](https://clang.llvm.org) >= version 18

#### Note: for Clang users
You may want to add the following argument to `meson setup` or `meson configure` 
to use [libc++](https://libcxx.llvm.org):
```sh
-D use-libcxx=true
```

Building
========
```sh
git clone https://codeberg.org/wreedb/confidant.git && cd confidant

meson setup .build --buildtype=release # --prefix=/my/prefix
# to see/modify options:
meson configure .build
meson compile -C .build
meson install -C .build

# to check that installation was successful
confidant help
confidant version
```

Runtime dependencies
====================
- [libucl](https://github.com/vstakhov/libucl)

Meson will attempt to build *libucl* on-demand if not found on your system.


Flake
=====
Nix users can try **Confidant** directly with the following commands:
```sh
nix run git+https://codeberg.org/wreedb/confidant.git -- --version
nix run git+https://codeberg.org/wreedb/confidant.git -- --help
```