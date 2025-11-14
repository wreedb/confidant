project(
  'confidant',
  'cpp',
  version: files('.version'),
  meson_version: '>= 1.4.0',
  default_options: ['cpp_std=c++23'],
)

config_hpp = configuration_data()
test_hpp = configuration_data()

config_hpp.set_quoted('PROJECT_VERSION', meson.project_version())
config_hpp.set_quoted('PROJECT_NAME', meson.project_name())

test_hpp.set_quoted('PROJECT_ROOT', meson.project_source_root())

configure_file(output: 'config.hpp', configuration: config_hpp)
configure_file(output: 'test.hpp', configuration: test_hpp)

if get_option('static-stdlib')
    add_project_link_arguments('-static-libgcc', '-static-libstdc++', language: 'cpp')
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
    'src/xdg.cpp',
    'src/util.cpp',
    'src/ucl.cpp',
    'src/confidant.cpp',
    'src/logging.cpp',
    'src/help.cpp',
    'src/main.cpp'
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