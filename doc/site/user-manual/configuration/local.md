<!--
SPDX-FileCopyrightText: 2025 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Local Configuration
**Local** (meaning they work on local basis; the directory they reside 
in) configuration files are where you declare your *dotfile* symlink paths.
For a quick look at **Confidant** in action, see the [example](#a-real-world-example)!
They offer three `node` types, `repostiory`, `links` and `templates`.

## `repository`
Holds metadata about your repository.

| Name | Type     | Required | Default |
|------|----------|----------|---------|
|`url` | `string` | `false`  | empty   |

### `url`
If used, this should point to the page for your repository on your version 
control host, such as `https://codeberg.org/wreedb/config.git`.

## `links`
Explicit declarations of individual links to create with fine-grain control.

Each node within `links` requires a name, it can be anything; though duplicate 
names will result in only the last occurrence being used. For example, the 
following depicts a node named `neovim`.
```
links: {
    neovim: {
        source: ${repo}/.config/nvim
        destdir: ${xdg_config_home}
        type: directory
    }
}
```

Each node has the following fields:

| Name                | Type     | Required             | Default |
|---------------------|----------|----------------------|---------|
| `source`            | `path`   | `true`               | none    |
| `dest` or `destdir` | `path`   | `true`               | none    |
| `type`              | `string` | only for directories | `file`  |

### `source`

The source file for which to create a symlink. This almost always will be a 
path to a file (or directory) within your repository.

### `dest` or `destdir`

These determine the target path of the symlink to create, giving you two 
different ways to approach it. Using `dest` requires the path to be written 
in full, such as `${XDG_CONFIG_HOME}/foo/bar.conf`. In contrast, using `destdir` 
allows **Confidant** to reuse the *basename* of the `source` file or directory 
to determine the symlink path.
```
links: {
    emacs: {
        source: ${repo}/.config/emacs/init.el
        dest: ${home}/.config/emacs/init.el
    }
}
```
In this example, the symlink will be placed at `${HOME}/.config/emacs/init.el`.

!!! warning "Conflict"
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
```
templates: {
    home: {
        source: ${repo}/%{item}
        dest: ${home}/%{item}
        items: [
            .bashrc
            .profile
        ]
    }
    config: {
        source: ${repo}/.config/%{item}
        dest: ${xdg_config_home}/%{item}
        items: [
            nvim/init.lua
            kitty/kitty.conf
            fish/conf.d
            fish/config.fish
        ]
    }
}
```
Each node of course, similar to `links`, requires a name; naming them after 
where the template points to is a good practice (such as `home` for one that 
points to your home directory).


| Name     | Type            | Required | Default |
|----------|-----------------|----------|---------|
| `source` | `path`          | `true`   | none    |
| `dest`   | `path`          | `true`   | none    |
| `items`  | `list of paths` | `true`   | none    |

The `source` and `dest` should each contain a path ending with the `%{item}` 
identifier. This identifier will be expanded to each item in `items`.

For a quick demonstration, the [example at the start of this section](#templates) would 
result in the following symlinks being created (with `${repo}` being a placeholder for 
the directory your `confidant.ucl` is in):
```sh
${repo}/.bashrc                  -> ${HOME}/.bashrc
${repo}/.profile                 -> ${HOME}/.profile
${repo}/.config/nvim/init.lua    -> ${XDG_CONFIG_HOME}/nvim/init.lua
${repo}/.config/kitty/kitty.conf -> ${XDG_CONFIG_HOME}/kitty/kitty.conf
${repo}/.config/fish/conf.d      -> ${XDG_CONFIG_HOME}/fish/conf.d
${repo}/.config/fish/config.fish -> ${XDG_CONFIG_HOME}/fish/config.fish
```
!!! note
    Template node items are automatically inferred as a `directory` or `file` 
    based on the type of file the item would point to after being substituted 
    into the `source` path.


## tags
(since 0.3.0) Both `templates` and `links` nodes may optionally contain a 
`tag` field. The value specified for a tag is a simple string name, such as 
`desktop` or `work`. These entries are skipped when running `confidant link`, 
and are only used when you provide the `-t,--tags` argument, specifying them 
by name.

Tags are particularly useful when you have multiple versions of a file, which 
would all occupy the same destination, but in different contexts or on different
machines. Here is a quick example:

```
links: {
    wireplumber-laptop: {
        source: ${repo}/.config/wireplumber/wireplumber.conf.d/51-local.conf.laptop
        dest: ${xdg_config_home}/.config/wireplumber/wireplumber.conf.d/51-local.conf
        tag: laptop
    };

    wireplumber-desktop: {
        source: ${repo}/.config/wireplumber/wireplumber.conf.d/51-local.conf.desktop
        dest: ${xdg_config_home}/.config/wireplumber/wireplumber.conf.d/51-local.conf
        tag: desktop
    };
};
```
My desktop and laptop have different audio hardware configurations, and thus I 
use two different wireplumber configuration files. Given that my local 
configuration contains the above snippet, I would run:

```sh
confidant link -t desktop
```
on my desktop computer, and:
```sh
confidant link -t laptop
```
on my laptop. The two files can coexist peacefully in my repository and be 
properly managed on a contextual basis.

!!! warning "Conflicting Tags"
    Since **Confidant** does no state management, this means that it is 
    possible to specify two tags which would place different files in the 
    same destination, which will result in an error. Since tags are never 
    utilized without explicitly passing them on the command-line; this is 
    expected behavior, as this action would be attempting to overwrite an 
    existing link.

# A real-world example
If you learn better by example, you can inspect my [personal dotfiles repository](https://codeberg.org/wreedb/config.git)
for a look at how **Confidant** can be used.