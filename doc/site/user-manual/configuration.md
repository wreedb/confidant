<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# Configuration

**Confidant** uses [`libucl`](https://github.com/vstakhov/libucl) for configuration, which
provides a very comfortable, flexible and easy to use syntax.

<pre class="ucl-code">
<span class="node-key">repository</span>: {
    <span class="keyword">url</span>: <span class="string">https://codeberg.org/wreedb/config.git</span>
}

<span class="node-key">links</span>: {
    <span class="node-key">neovim</span>: {
        <span class="keyword">source</span>: <span class="string"><span class="delim">${</span><span class="variable">repo</span><span class="delim">}</span>/.config/nvim</span>
        <span class="keyword">dest</span>: <span class="string"><span class="delim">${</span><span class="variable">HOME</span><span class="delim">}</span>/.config</span>
        <span class="keyword">type</span>: <span class="string">directory</span>
    }
}
</pre>

As you may notice, there are [string-interpolation variables](https://en.wikipedia.org/wiki/String_interpolation);
They are built-in, and **Confidant** provides a number of them for convenience. 
All of the variables defined here can be written in full upper or lower case.
Below is a table of the different variables provided for use in the configuration.


| Name | Value |
|------|-------|
| `REPO` | The directory containing your local configuration file |
| `HOME` | Your home directory, also known as `~` |
| `XDG_CONFIG_HOME` | The value of the `XDG_CONFIG_HOME` environment variable when present, or `$HOME/.config` |
| `XDG_CACHE_HOME` | The value of the `XDG_CACHE_HOME` environment variable when present, or `$HOME/.cache` |
| `XDG_DATA_HOME` | The value of the `XDG_DATA_HOME` environment variable when present, or `$HOME/.local/share` |
| `XDG_STATE_HOME` | The value of the `XDG_STATE_HOME` environment variable when present, or `$HOME/.local/state` |
| `XDG_RUNTIME_DIR` | The value of the `XDG_RUNTIME_DIR` environment variable when present, or `/run/user/$(id -u)` |
| `USER` | The value of the `USER` environment variable |