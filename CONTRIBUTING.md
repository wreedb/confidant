<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>
SPDX-License-Identifier: GPL-3.0-or-later
-->

# Contributing to Confidant
If you'd like to contribute to **Confidant**, you must agree that any and all 
merged contributions will be licensed under the **GNU General Public License** 
version 3 (or later). For information about this license, see the [license text](./LICENSE) 
included in the project source, or the corresponding page on [gnu.org](https://gnu.org/licenses/gpl-3.0.html)

## Use of third-party code
If your contributions make use of (or vendor) any third-party code, you must 
ensure that the code in question is compatible for use in a project using  
**GPLv3**. You must also make sure to enact any requirements made by the 
third-party license; such as including its license alongside **Confidant**. This 
project takes licensing extremely serious, and any contributions must reflect 
this.

## Keeping track
**Confidant** adheres to the [REUSE](https://reuse.software) specification; 
Changes made should be annotated in-file or in `REUSE.toml` in the 
projects' root directory. See the [specification](https://reuse.software/spec/) and [FAQ](https://reuse.software/faq/) for more information. 
All potential contributors should use the [REUSE tool](https://github.com/fsfe/reuse-tool) to run 
`reuse lint` before making submissions.

## Why are these policies so strict?
Some developers wish to free themselves of interaction with copyright and 
licensing matters, however the idea of such freedom is not obtained through 
a hands-off approach. Freedom is not granting all to do as they wish; 
it is granting all to do as they wish without encroaching on others' freedom 
or having their freedom encroached upon.

## Exceptions
There are absolutely no exceptions.

## Style
**Confidant** follows the [Conventional Commits](https://conventionalcommits.org/en/v1.0.0/#summary) specification, and therefore 
contributions must also. For ease of use, the author recommends using the 
[convco](https://convco.github.io) tool for commits, as it helps streamline the process. Following these 
rules allows the project to automatically generate changelogs, compatible with
the [Keep a Changelog](https://keepachangelog.com) specification.