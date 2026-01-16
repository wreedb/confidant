<!--
SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: GPL-3.0-or-later
-->
# Changelog
## [v0.4.5] - 2026-01-16


### Changed

-  Update changelog

-  Add .clangd, add ignores for unused includes to .clang-tidy

-  Auto insert final newlines for everything except for `.version`

-  Update translations

-  Bump to 0.4.5, update copyright year, adjust formatting for consistency

-  Add .clangd, add ignores for unused includes to .clang-tidy

-  Auto insert final newlines for everything except for `.version`

-  Update translations

-  Bump to 0.4.5, update copyright year, adjust formatting for consistency

-  Merge branch 'main' of ssh://codeberg.org/wreedb/confidant

-  Ignore merge commits in cliff.toml


### Fixed

-  Format string error

-  Add a space

-  Add a space

## [v0.4.3] - 2025-11-25


### Fixed

-  Incorrect localedir in executables

## [v0.4.2] - 2025-11-25


### Changed

-  Make aur repo into submodule

-  Updated changelog

-  Move examples to misc

-  Integrating gettext

- **i18n**: Adding auxilary help for gettext

-  Add translation section to readme, credit lyra on thanks

-  Split meson.build to subdirs

-  Move headers into src, no need for separate include

-  Add gettext dependency to flake

- **i18n**: Update po files, complete es_MX translation

-  Change some strings in help.cpp for translated formatting

-  Reuse annotations

-  Fix typo in THANKS.md

-  Repo maint, translations updated

-  Add gettext configuration data to meson.build, check for localedir being undefined

-  Add glibc locales to flake native inputs

- **i18n**: Ever-changing po updates

- **i18n**: Pt_BR translations added, marked for install with meson

-  Formatting change

-  Add language independent spacing for translations to use for alignment

-  Bump to 0.4.2


### Fixed

-  No longer copy string maps in parse.cpp, write them in place

-  Add fallback locale settings when environment is isolated

## [v0.4.1] - 2025-11-24


### Changed

-  Update PKGBUILD and changelog

-  Add links to my dotfiles repo for example usage people can look at

-  Change log-level logic for warnextra function

-  Updating reuse annotations

-  Bump to 0.4.1


### Fixed

-  Added custom subproject meson.build for lyra

-  Regression; create global configuration file with defaults, or error with a better message

## [v0.4.0] - 2025-11-23


### Changed

-  Update CHANGELOG and arch PKGBUILD

-  Set up build infra for migration to lyra

-  Fully switch out main to use lyra

-  Separate variable map creation for UCL parsing

-  Add -g --global to help messages

-  Update manpage with json options

-  Add 'vhs' tape demo gif and mp4

-  Update flake

-  Updated completions and usage grammar

-  Add documentation for the json dump flag, add demonstration gif to site

-  Bump to 0.4.0


### Fixed

-  Detect when stdout is not a terminal and omit ansi escapes

-  Amend incorrect init file path output message


### Feat

-  Dump output to JSON option added

## [v0.3.0] - 2025-11-21


### Changed

-  Update changelog for 0.2.0

-  Fix README formatting

-  Added tests for local and global config serializing

-  Update docs for tag feature

-  Update completions with --tags,-t option

-  Update get function to handle tags

-  Reuse upkeep, bump to 0.3.0


### Feat

-  Conditional tags added to support multiple versions of the same file

## [v0.2.0] - 2025-11-21


### Changed

-  Update install instructions

-  PKGBUILD added

-  Add scripts for clang compilation testing

-  Logging now fmt and msg, ansi header separate

-  Add fully static meson option, update clang build process

-  Bump to 0.2.0, updated readme


### Fixed

-  Fix config-dump displaying incorrect source for links

## [v0.1.1] - 2025-11-19


### Changed

-  Added section 1 manual page

-  Added CHANGELOG post 0.1.0, update install.md for site v.s. repo

-  REUSE upkeep

-  Update nix flake to include manpages, add man(1) page to meson.build

-  Fixed some formatting for the mkdocs site

-  Bump to 0.1.1


### Fixed

-  Correctly handle broken symlink destination paths

## [v0.1.0] - 2025-11-19


### Changed

-  Initialize repo

-  Update REUSE

-  Add logging functions

-  Improving CLI experience

-  Options.hpp with namespace option hierarchy

-  Added testing helper scripts and config

-  Convert println to internal logging function

-  Config setting serialization tests added

-  Update README, added AUTHORS, CONTRIBUTING, THANKS, INSTALL

-  Version file now at .version

-  Added cliff.toml for changelog generation

-  Update meson.build to reflect version file change

-  Indentation fix for logging.hpp

-  Use std::unreachable for optimization

-  Fix building with clang and libcxx

-  Add instructions for running the flake directly with nix

-  Update REUSE, add annotation headers

-  Changing templated links to a vector type

-  Added std::optional get functions for parsing ucl, replace less safe direct calls

-  Update completions and .usage file

-  Make meson tests conditional

-  Added section 5 manual page

-  Move serializing into parse.cpp and hpp

-  Added mkdocs website

-  Add quiet option to completions

-  Updated flake

-  Update README example


### Fixed

-  Update flake to work with nix run


### Feat

-  Link, link-from functions added

-  Full cli help menu

-  Flake build added

-  Added shell completions for fish

-  Add bash/zsh completions using complgen .usage grammar

-  Get command added, color added to help interface

<!-- Generated by git-cliff, compliant with keepachangelog.com standard -->
