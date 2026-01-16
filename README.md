<!--
SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>

SPDX-License-Identifier: GPL-3.0-or-later
-->

# Confidant
*Your configuration pal and confidant*

## What is it?
Confidant is a configuration or "dotfile" manager, helping you keep your 
configuration files clean, consistent and sensible across machines.

For example, if you use your terminal and shell alot, it would be nice 
to have the same configuration on both your laptop, desktop or even 
your work machine.

## How does it work?
**Confidant** uses a simple configuration syntax from [libucl](https://github.com/vstakhov/libucl) to
define which configuration files you want to keep track of, *where* you want 
them, and *how* you want them. You place them inside of a repository, define 
them in your `confidant.ucl`, and **Confidant** will do the rest!

The underlying method of management is the use of symbolic links, similar to 
what [GNU Stow](https://gnu.org/software/stow) does, but with a slightly more 
user-friendly interface, and tailored especially towards managing configuration 
files. In some ways it is also influenced by [Ansible playbooks](https://redhat.com/en/topics/automation/what-is-an-ansible-playbook), in that there is
(simple) support for template syntax, to make your time writing `confidant.ucl` 
less verbose. See [THANKS.md](THANKS.md) for more attribution about influences 
and inspirations.

## Installation
See [INSTALL.md](INSTALL.md) for instructions on installing and/or building

## Example Usage
For a real-world example, [my dotfiles](https://codeberg.org/wreedb/config.git) are using already. 

You will first want to initialize a repository:
```sh
mkdir ~/projects/dotfiles
cd ~/projects/dotfiles
confidant init
```
Then you will want to modify the configuration file, `confidant.ucl`:
```
# ~/projects/dotfiles/confidant.ucl

create-directories: true

repository {
    url: https://github.com/user/dotfiles.git
}

links {
    nvim {
        type: directory
        source: ${repo}/.config/nvim
        destdir: ${xdg_config_home}
    }
    
    emacs {
        source: ${repo}/.config/emacs/init.el
        dest: ${xdg_config_home}/emacs/init.el
        tag: work
    }
}

templates: {
    config: {
        from: ${repo}/.config/%{item}
        to: ${home}/%{item}
        items: [
            .bashrc
            .profile
            .config/zsh
            .config/fontconfig/fonts.conf
        ]
    }
}
```
With a configuration file (like the above example), you would run:
```
confidant link
```
To apply the configuration. The above example would result in the following 
(where `${REPO}` expands to the repository directory you've made):
- directory `~/.config/nvim` symlinked from `${REPO}/.config/nvim`
- files `~/.bashrc` and `~/.profile` symlinked from `${REPO}/.bashrc` and `${REPO}/.profile`
- directory `~/.config/zsh` symlinked from `${REPO}/.config/zsh`
- file `~/.config/fontconfig/fonts.conf` symlinked from `${REPO}/.config/fontconfig/fonts.conf`

To apply tagged links, such as the `emacs` node; you would pass:
```
confidant link --tags work
```
For more documentation and examples, see the [website](https://wreedb.github.io/confidant)!

## Translations
This project uses [GNU Gettext](https://gnu.org/software/gettext) to be translated to other languages.
I would greatly appreciate any and all contributions for translations, 
regardless of how much you can provide; there's no amount of help too small!
If you or someone you know might be interested in helping out, feel free to 
make pull requests with your contributions. If you are unfamiliar with git, 
version control or other factors but would still like to contribute, please 
reach out to me, and we can try to work out a solution! My contact 
information is in the [AUTHORS](AUTHORS.md) document. I would be very greatful.

#### See Also

[Install/build instructions](INSTALL.md)  
[Contibutor info](CONTRIBUTING.md)  
[Releases](https://codeberg.org/wreedb/confidant/releases)  
[Inspirations and acknowledgments](THANKS.md)  
[Contact information](AUTHORS.md)  
[The license](LICENSE.md)  