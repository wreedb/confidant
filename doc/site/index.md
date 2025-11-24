Confidant
=========

Your configuration *pal and confidant*.

A configuration (or *dotfile*) management tool, designed to be easy to use, 
fast, focused and reliable.

![demonstration](assets/demo.gif)

The Problem
-----------
Your configuration files are expected to be somewhere, and keeping track of 
them with a version control system is not simple.

The Solution
------------
A repository containing your *real* files, and a program that manages creating 
symbolic links where your files need to be, with as little effort needed as 
possible.

A Quick Look
------------
If you just want to see **Confidant** in action, you can take a look at my 
personal [dotfiles repository](https://codeberg.org/wreedb/config.git) for a real-world example
of how **Confidant** can be used.

Why **Confidant**?
------------------
It combines the power of simple templating inspired by Ansible with the 
practical symlink-based approach of GNU Stow to create a robust tool that 
I (the author, biased) think is the Goldilocks of configuration management.

In addition to its' functionality, I have put extensive time and effort into 
making the user experience as pleasant and considerate as possible. **Confidant** 
follows the [XDG basedir](https://specifications.freedesktop.org/basedir/latest) specification and [no-color](https://no-color.org) community standard out 
of the box. I've also provided and maintain up-to-date command-line shell 
completions files for [Fish](https://fishshell.com), [Bash](https://gnu.org/software/bash) and [Zsh](https://zsh.org).


Background
----------
I was unsatisfied with the choices on offer for this task, and there are a few 
options one might choose. For a very long time, I was using [GNU Stow](https://gnu.org/software/stow); which 
is a brilliant piece of software, but it was designed as a symlink farm manager 
in a generic sense, and lacks some usability features for the task of managing 
configuration files. I then tried both [Chezmoi](https://chezmoi.io) and [Yadm](https://yadm.io) but found them both 
to be heavy-handed and seemed like far more than what I needed.

I was left wanting. For a short time I used [Ansible](https://www.redhat.com/en/ansible-collaborative), and I quite liked the 
templating features. However, Ansible covers a wide scope, it does IT 
automation, software deployment and complex orchestration. These are all great 
features, but usage of Ansible felt very slow, which makes sense since it 
is a very complex and versatile tool.

At that point, a lightbulb appeared above my head, using my abilities writing 
C++ to make a very fast, simple and easy to use configuration manager. One that 
provides the power of simple templating and a friendly interface. Well, here 
we are.