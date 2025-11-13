project(
  'confidant',
  'cpp',
  version: files('meson.version'),
  meson_version: '>= 1.1',
  default_options: ['cpp_std=c++23'],
)

config_hpp = configuration_data()

config_hpp.set_quoted('PROJECT_VERSION', meson.project_version())
config_hpp.set_quoted('PROJECT_NAME', meson.project_name())

configure_file(output: 'config.hpp', configuration: config_hpp)

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

deps = [libucl_dep, cli11_dep]

executable(
  'confidant',
  sources,
  include_directories: ['include'],
  install: true,
  dependencies: deps
)