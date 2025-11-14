project(
  'confidant',
  'cpp',
  version: files('.version'),
  meson_version: '>= 1.4.0',
  default_options: ['cpp_std=c++23'],
)

cxx = meson.get_compiler('cpp')

if get_option('static-stdlib')
    if get_option('use-libcxx') and cxx.get_id() == 'clang'
        # add_project_link_arguments('-static-libc++', language: 'cpp')
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
test_hpp = configuration_data()

config_hpp.set_quoted('PROJECT_VERSION', meson.project_version())
config_hpp.set_quoted('PROJECT_NAME', meson.project_name())

test_hpp.set_quoted('PROJECT_ROOT', meson.project_source_root())

configure_file(output: 'config.hpp', configuration: config_hpp)
configure_file(output: 'test.hpp', configuration: test_hpp)

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
    'src/xdg.cpp',
    'src/util.cpp',
    'src/ucl.cpp',
    'src/confidant.cpp',
    'src/logging.cpp',
    'src/help.cpp',
)

test_sources = files(
    'test/config-createdirs.cpp',
    'test/config-links.cpp'
)

deps = [libucl_dep, cli11_dep]

support_lib = static_library(
    'support',
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
  link_with: support_lib
)

foreach n : test_sources
    name = n.full_path().split('/')[-1].split('.')[0]
    test_exe = executable(
        name,
        n,
        dependencies: deps,
        include_directories: ['include'],
        link_with: support_lib)
    test(name, test_exe)
endforeach