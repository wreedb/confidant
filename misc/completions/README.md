# Confidant - shell completions

Docs:   https://wreedb.github.io/confidant
Source: https://codeberg.org/wreedb/confidant


## Install Instructions
---
#### Bash

Copy [confidant.bash](bash/confidant.bash) into one of the following 
directories:
```sh
${HOME}/.local/share/bash-completion/completion
/usr/share/bash-completion/completion
```

And then restart your shell.

---
#### Zsh

Copy '_confidant' into a directory in your `$fpath`, to see the 
directories in yours, run:
```sh
print -l $fpath
```
You will likely need to restart your shell and/or re-run `compinit`

---
### Fish

Copy 'confidant.fish' into one of the following directories:
```sh
${HOME}/.config/fish/completions
${HOME}/.local/share/fish/vendor_completions.d
/usr/share/fish/vendor_completions.d
```