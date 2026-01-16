<!--
SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Command-line interface


## Common Flags
These flags may be provided with practically any of **Confidant**s' subcommands.


### `-q,--quiet`
Silence most (non-fatal) message output.

### `-v,--verbose`
Immediately sets `log-level` to `trace`, producing all possible output.

### `-?,-h,--help`
Display help information for **Confidant** or one of its' subcommands.


## Commands
The command-line interface was crafted manually and meticulously to provide 
a smooth experience for all users. It is highly recommended to ensure you 
install the command-line shell completion file for the shell you use if 
available.

### `init`

Will provide you a sample (local) configuration file in your current directory, 
or a directory you specify at the end of the command. The sample is documented 
with comments explaining how the different parts work, though if you need 
further guidance, see the [global](configuration/global.md) and [local](configuration/local.md) configuration pages.
May be passed `-d,--dry-run` to simulate actions.

### `link`

Performs the main symlinking action of **Confidant**, reading your local 
configuration and applying the links you've declared. If you are unsure 
about the correctness of your declarations, pass the `-d,--dry-run` flag
alongside the command to simulate the actions that *would* be taken before 
really applying them.

For use of tags, pass the `-t,--tags` option with a tag name or comma 
separated list of names, such as:
```
confidant link -t desktop,work
```
This will include links and templates containing the `desktop` or `work` tag.
This is particularly useful for situations where you have multiple files that 
would occupy the same destination, but are meant to be used on different 
machines or contexts, see [tags](configuration/local.md#tags) for more 
information and examples of tag usage.

### `config`

Allows you to display your configuration and get a *birds-eye* view of 
your settings

#### `dump`

Pretty-prints your local configuration settings, though you may also pass
`-g,--global` to display the settings of your global configuration instead.
(since 0.4.0) You may pass the `-j,--json` option to output either configuration
in [JSON](https://json.org) format.

#### `get`

Queries your configuration settings given the name of a section, such as 
`repository`, `links` or `templates`. Also allows you to query nested sections 
by separating the section names with periods; for example:
```sh
confidant config get repository.url
confidant config get templates.config.items
```
To query your global configuration settings, pass `-g,--global`

### `help <command>`

Display help information about sub-commands, supported arguments include:
`init`, `config`, `config dump`, `config get` and `link`.

### `usage`

Display brief command-line usage information.

### `version`

Display brief version information.