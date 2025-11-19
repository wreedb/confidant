<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Configuration

**Confidant** uses [`libucl`](https://github.com/vstakhov/libucl) for configuration, which
provides a very comfortable, flexible and easy to use syntax.
```
repository: {
    url: https://codeberg.org/wreedb/config.git
}

links: {
    neovim: {
        source: ${repo}/.config/nvim
        dest: ${HOME}/.config
        type: directory
    }
}
```
As you may notice, there are [string-interpolation variables](https://en.wikipedia.org/wiki/String_interpolation);
They are built-in, and **Confidant** provides a number of them for convenience. 
All of the variables defined here can be written in full upper or lower case.
Below is a list of the different variables provided for use in the configuration.

---
### `REPO` or `repo`
The directory containing your local configuration file

---
### `HOME` or `home`
Your home directory

---
### `USER` or `user`
The value of `USER` in the environment, or the invoking users' `passwd` 
username entry, equivalent to `$(id -un)`.

---
### `XDG_CONFIG_HOME` or `xdg_config_home`
The value of `XDG_CONFIG_HOME` in the environment, or 
alternatively `${HOME}/.config`.

---
### `XDG_CACHE_HOME` or `xdg_cache_home`
The value of `XDG_CACHE_HOME` in the environment, or 
alternatively `${HOME}/.cache`.

---
### `XDG_DATA_HOME` or `xdg_data_home`
The value of `XDG_DATA_HOME` in the environment, or 
alternatively `${HOME}/.local/share`.

---
### `XDG_STATE_HOME` or `xdg_state_home`
The value of `XDG_STATE_HOME` in the environment, or 
alternatively `${HOME}/.local/state`.

---
### `XDG_RUNTIME_DIR` or `xdg_runtime_dir`
The value of `XDG_RUNTIME_DIR` in the environment, or 
alternatively `/run/user/$(id -u)`.