project(
    'confidant', 'cpp',
    version: files('.version'),
    license: 'GPL-3.0-or-later',
    license_files: 'LICENSE.md',
    meson_version: '>= 1.4.0',
    default_options: ['cpp_std=c++23'])

cxx = meson.get_compiler('cpp')

if get_option('static-stdlib')
    if get_option('use-libcxx') and cxx.get_id() == 'clang'
        # clang doesn't have a nice -static-libc++ flag sadly
        add_project_link_arguments('-l:libc++.a', '-l:libc++abi.a', language: 'cpp')
    else
        add_project_link_arguments('-static-libstdc++', language: 'cpp')
    endif
    add_project_link_arguments('-static-libgcc', language: 'cpp')
endif

if get_option('use-libcxx') and cxx.get_id() == 'clang'
    add_project_arguments('-stdlib=libc++', language: 'cpp')
    add_project_link_arguments('-stdlib=libc++', language: 'cpp')
endif

config_hpp = configuration_data()

config_hpp.set_quoted('PROJECT_VERSION', meson.project_version())
config_hpp.set_quoted('PROJECT_NAME', meson.project_name())


configure_file(output: 'config.hpp', configuration: config_hpp)

# shell completions
if 'bash' in get_option('shell-completions')
    install_data(
        'misc/completions/bash/confidant.bash',
        install_dir: get_option('datadir') / 'bash-completion' / 'completions')
endif

if 'fish' in get_option('shell-completions')
    install_data(
        'misc/completions/zsh/_confidant',
        install_dir: get_option('datadir') / 'zsh' / 'site-functions')
endif

if 'zsh' in get_option('shell-completions')
    install_data(
        'misc/completions/fish/confidant.fish',
        install_dir: get_option('datadir') / 'fish' / 'vendor_completions.d')
endif

manpages_feat = get_option('man-pages')

if manpages_feat.enabled()
    scdoc = find_program('scdoc', required: true)
    if scdoc.found()
        date_month = run_command('date', '+%B', check: true).stdout().strip()
        date_year = run_command('date', '+%Y', check: true).stdout().strip()
        manconf = configuration_data()
        manconf.set('MONTH', date_month)
        manconf.set('YEAR', date_year)
        manconf.set('VERSION', meson.project_version())
        confidant_ucl_5 = configure_file(
            input: 'doc/man/confidant.ucl.5.scd.in',
            output: 'confidant.ucl.5.scd',
            configuration: manconf)
        confidant_1 = configure_file(
            input: 'doc/man/confidant.1.scd.in',
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
endif

cli11_dep = dependency(
    'CLI11',
    required: false,
    fallback: ['cli11', 'CLI11_dep']
)

libucl_dep = dependency(
    'libucl',
    required: false,
    fallback: ['libucl', 'libucl_dep']
)

sources = files(
    'src/parse.cpp',
    'src/settings.cpp',
    'src/settings/global.cpp',
    'src/xdg.cpp',
    'src/util.cpp',
    'src/ucl.cpp',
    'src/confidant.cpp',
    'src/logging.cpp',
    'src/help.cpp',
)

deps = [libucl_dep, cli11_dep]

confidant_lib = static_library(
    'confidant',
    sources,
    include_directories: ['include'],
    dependencies: deps
)

executable(
  'confidant',
  'src/main.cpp',
  include_directories: ['include'],
  install: true,
  dependencies: deps,
  link_with: confidant_lib
)

if get_option('build-tests')
    test_hpp = configuration_data()
    test_hpp.set_quoted('PROJECT_ROOT', meson.project_source_root())
    configure_file(output: 'test.hpp', configuration: test_hpp)
    
    test_sources = files('test/config-createdirs.cpp')

    foreach n : test_sources
        name = n.full_path().split('/')[-1].split('.')[0]
        test_exe = executable(
            name,
            n,
            dependencies: deps,
            include_directories: ['include'],
            link_with: confidant_lib)
        test(name, test_exe)
    endforeach

endif