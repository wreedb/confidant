Confidant
=========
*Your configuration pal and confidant*

What is it?
-----------
Confidant is a configuration or "dotfile" manager, helping you keep your 
configuration files clean, consistent and sensible across machines.

For example, if you use your terminal and shell alot, it would be nice 
to have the same configuration on both your laptop, desktop or even 
your work machine.

How does it work?
-----------------
**Confidant** uses a simple configuration syntax from [libucl](https://github.com/vstakhov/libucl) to
define which configuration files you want to keep track of, *where* you want 
them, and *how* you want them. You place them inside of a repository, define 
them in your `confidant.ucl`, and **Confidant** will do the rest!

The underlying method of management is the use of symbolic links, similar to 
what [GNU Stow](https://gnu.org/software/stow) does, but with a slightly more 
user-friendly interface, and tailored especially towards managing configuration 
files. In some ways it is also influence by [Ansible playbooks](https://redhat.com/en/topics/automation/what-is-an-ansible-playbook), in that there is
(some) support for template syntax, to make your time writing `confidant.ucl` 
less verbose.