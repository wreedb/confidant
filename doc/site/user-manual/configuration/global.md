<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: GPL-3.0-or-later
-->

Global Configuration File
=========================

Global configurations are searched for from the following paths:

* `${XDG_CONFIG_HOME}/confidant/config.ucl`
* `${HOME}/.config/confidant/config.ucl`

!!! note
    **Confidant** will *NEVER* put it's own configuration files
    directly in your home directory, you deserve better than that.

Fields
------

The setting available to the global configuration are as follows:

#### `create-directories`
Type: `boolean`  
Default: `true`  

Controls the behavior of **Confidant** regarding the creation of directories. 
By default, if the parent directory of a symlink destination does not exist, 
**Confidant** will create it. This can also be used in local configuration 
files, which will override the global setting.

#### `log-level`
Type: `string` or `integer`  
Default: `normal` or `1`  
Valid values: `quiet`, `normal`, `info`, `debug`, `trace` or `0-4`  

Controls the amount of information displayed by **Confidant** at runtime. 
The default `normal` will only output information about errors encountered 
and symlinks created.

#### `color`
Type: `boolean`  
Default: `true`  

By default, **Confidant** uses Ansi escape sequences to sensibly colorize and 
format the text it outputs, however this may not be desired by all users. 
Setting this to `false` will disable the use of color and formatting. This can 
also be achieved by using the `NO_COLOR` environment variable to any value 
other than an empty string, as described by the [no-color](https://no-color.org) standard.

!!! note
    The environment variable will **always** override the configuration 
    file setting.