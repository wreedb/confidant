if get_option('build-tests')
    # set up test config header
    test_hpp = configuration_data()
    test_hpp.set_quoted('PROJECT_ROOT', meson.project_source_root())
    test_hpp.set_quoted('PROJECT_NAME', meson.project_name())
    test_hpp.set_quoted('PROJECT_VERSION', meson.project_version())
    configure_file(output: 'test.hpp', configuration: test_hpp)
    incdir = include_directories('../src')
    # test sources
    test_sources = files(
        'config-serialize-local.cpp',
        'config-serialize-global.cpp'
    )
    # make test executables
    foreach t : test_sources
        name = t.full_path().split('/')[-1].split('.')[0]
        test(name, executable(
            name, t,
            dependencies: deps,
            link_with: confidant_lib,
            include_directories: [incdir]))
    endforeach
endif