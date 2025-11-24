manpages_feat = get_option('man-pages')
scdoc = find_program('scdoc', required: manpages_feat)

if scdoc.found() and manpages_feat.allowed()
    date_month = run_command('date', '+%B', check: true).stdout().strip()
    date_year = run_command('date', '+%Y', check: true).stdout().strip()
    manconf = configuration_data()
    manconf.set('MONTH', date_month)
    manconf.set('YEAR', date_year)
    manconf.set('VERSION', meson.project_version())
    confidant_ucl_5 = configure_file(
        input: 'confidant.ucl.5.scd.in',
        output: 'confidant.ucl.5.scd',
        configuration: manconf)
    confidant_1 = configure_file(
        input: 'confidant.1.scd.in',
        output: 'confidant.1.scd',
        configuration: manconf)
    custom_target(
        'confidant.ucl.5',
        input: confidant_ucl_5,
        output: 'confidant.ucl.5',
        command: [scdoc],
        feed: true,
        capture: true,
        install: true,
        install_dir: get_option('mandir') / 'man5')
    custom_target(
        'confidant.1',
        input: confidant_1,
        output: 'confidant.1',
        command: [scdoc],
        feed: true,
        capture: true,
        install: true,
        install_dir: get_option('mandir') / 'man1')
endif