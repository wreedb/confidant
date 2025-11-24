if 'bash' in get_option('shell-completions')
    install_data(
    'bash/confidant.bash',
    install_dir: get_option('datadir') / 'bash-completion' / 'completions')
endif

if 'fish' in get_option('shell-completions')
    install_data(
    'zsh/_confidant',
    install_dir: get_option('datadir') / 'zsh' / 'site-functions')
endif

if 'zsh' in get_option('shell-completions')
    install_data(
    'fish/confidant.fish',
    install_dir: get_option('datadir') / 'fish' / 'vendor_completions.d')
endif