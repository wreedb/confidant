<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Local Configuration
**Local** (meaning they work on local basis; the directory they reside 
in) configuration files are where you declare your *dotfile* symlink paths.
They offer three `node` types, `repostiory`, `links` and `templates`.

## `repository`
Holds metadata about your repository.

| Name | Required | Default |
|------|----------|---------|
|`url` | `false`  | empty   |

### `url`
If used, this should point to the page for your repository on your version 
control host, such as `https://codeberg.org/wreedb/config.git`.

## `links`
Explicit declarations of individual links to create with fine-grain control.

Each node within `links` requires a name, it can be anything; though duplicate 
names will result in only the last occurrence being used. For example, the 
following example depicts a node named `neovim`.
<pre class="ucl-code">
<span class="node-key">links</span>: {
    <span class="node-key">neovim</span>: {
        <span class="keyword">source</span>: <span class="string"><span class="delim">${</span><span class="variable">repo</span><span class="delim">}</span>/.config/nvim</span>
        <span class="keyword">destdir</span>: <span class="delim">${</span><span class="variable">xdg_config_home</span><span class="delim">}</span>
        <span class="keyword">type</span>: <span class="string">directory</span>
    }
}
</pre>

Each node has the following fields:

| Name | Required | Default |
|------|----------|---------|
| `source`  | `true` | none   |
| `dest` or `destdir` | `true` | none   |
| `type` | only when defining a directory symlink | `file` |

### `source`

The source file for which to create a symlink. This almost always will be a 
path to a file (or directory) within your repository.

### `dest` or `destdir`

These determine the target path of the symlink to create, giving you two 
different ways to approach it. Using `dest` requires the path to be written 
in full, such as `${XDG_CONFIG_HOME}/foo/bar.conf`. In contrast, using `destdir` 
allows **Confidant** to reuse the *basename* of the `source` file or directory 
to determine the symlink path.
<pre class="ucl-code">
<span class="node-key">links</span>: {
    <span class="node-key">emacs</span>: {
        <span class="keyword">source</span>: <span class="string"><span class="delim">${</span><span class="variable">repo</span><span class="delim">}</span>/.config/emacs/init.el</span>
        <span class="keyword">destdir</span>: <span class="string"><span class="delim">${</span><span class="variable">home</span><span class="delim">}</span>/.config/emacs</span>
    }
}
</pre>
In this example, the symlink will be placed at `${HOME}/.config/emacs/init.el`.

!!! warning "conflict"
    `dest` and `destdir` are mutually exclusive; meaning in any one `links` node,
    you must use at least one, but never both.

### `type`
If not specified, all `links` nodes are assumed to be `file` type links. This 
means you only *need* to use this when declaring a directory symlink, though 
declaring it for your files as well doesn't hurt, and may make your 
configuration easier to read.

## `templates`
Somewhat similar to `links`, but provides a simple templating syntax using 
`%{item}` to substitute each item into the `source` and `dest`

<pre class="ucl-code">
<span class="node-key">templates</span>: {
    <span class="node-key">home</span>: {
        <span class="keyword">source</span>: <span class="string"><span class="delim">${</span><span class="variable">repo</span><span class="delim">}</span>/<span class="delim">%{</span><span class="variable">item</span><span class="delim">}</span></span>
        <span class="keyword">dest</span>: <span class="string"><span class="delim">${</span><span class="variable">home</span><span class="delim">}</span>/<span class="delim">%{</span><span class="variable">item</span><span class="delim">}</span></span>
        <span class="keyword">items</span>: [
            <span class="string">.bashrc</span>
            <span class="string">.profile</span>
        ]
    }
    <span class="node-key">config</span>: {
        <span class="keyword">source</span>: <span class="string"><span class="delim">${</span><span class="variable">repo</span><span class="delim">}</span>/.config/<span class="delim">%{</span><span class="variable">item</span><span class="delim">}</span></span>
        <span class="keyword">dest</span>: <span class="string"><span class="delim">${</span><span class="variable">xdg_config_home</span><span class="delim">}</span>/<span class="delim">%{</span><span class="variable">item</span><span class="delim">}</span></span>
        <span class="keyword">items</span>: [
            <span class="string">nvim/init.lua</span>
            <span class="string">kitty/kitty.conf</span>
            <span class="string">fish/conf.d</span>
            <span class="string">fish/config.fish</span>
        ]
    }
}
</pre>

Each node of course, similar to `links`, requires a name; naming them after 
where the template points to is a good practice (such as `home` for one that 
points to your home directory).


| Name  | Required | Default |
|-------|--------|---------|
| `source` | `true` | none |
| `dest` | `true` | none |
| `items` | `true` | none |

The `source` and `dest` should each contain a path ending with the `%{item}` 
identifier. This identifier will be expanded to each item in `items`.

For a quick demonstration, the [example at the start of this section](#templates) would 
result in the following symlinks being created (with `${repo}` being a placeholder for 
the directory your `confidant.ucl` is in):

<pre id="demonstration" class="ucl-code">
<span class="delim">${</span><span class="variable">repo<span class="delim">}</span><span class="string">/.bashrc                  -> <span class="delim">${</span><span class="variable">HOME<span class="delim">}</span><span class="string">/.bashrc
<span class="delim">${</span><span class="variable">repo<span class="delim">}</span><span class="string">/.profile                 -> <span class="delim">${</span><span class="variable">HOME<span class="delim">}</span><span class="string">/.profile
<span class="delim">${</span><span class="variable">repo<span class="delim">}</span><span class="string">/.config/nvim/init.lua    -> <span class="delim">${</span><span class="variable">XDG_CONFIG_HOME<span class="delim">}</span><span class="string">/nvim/init.lua
<span class="delim">${</span><span class="variable">repo<span class="delim">}</span><span class="string">/.config/kitty/kitty.conf -> <span class="delim">${</span><span class="variable">XDG_CONFIG_HOME<span class="delim">}</span><span class="string">/kitty/kitty.conf
<span class="delim">${</span><span class="variable">repo<span class="delim">}</span><span class="string">/.config/fish/conf.d      -> <span class="delim">${</span><span class="variable">XDG_CONFIG_HOME<span class="delim">}</span><span class="string">/fish/conf.d
<span class="delim">${</span><span class="variable">repo<span class="delim">}</span><span class="string">/.config/fish/config.fish -> <span class="delim">${</span><span class="variable">XDG_CONFIG_HOME<span class="delim">}</span><span class="string">/fish/config.fish
</pre>

!!! note
    Template node items are automatically inferred as a `directory` or `file` 
    based on the type of file the item would point to after being substituted 
    into the `source` path.