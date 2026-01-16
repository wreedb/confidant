# SPDX-FileCopyrightText: 2026 Will Reed <wreed@disroot.org>
# SPDX-License-Identifier: GPL-3.0-or-later

project(
    'confidant', 'cpp',
    version: files('.version'),
    license: 'GPL-3.0-or-later',
    license_files: 'LICENSE.md',
    meson_version: '>= 1.4.0',
    default_options: ['cpp_std=c++23'])

deps = []
cxx = meson.get_compiler('cpp')

opt_staticstdlib = get_option('static-stdlib')
opt_libcxx = get_option('use-libcxx')
opt_static = get_option('static')

if opt_static
    add_project_link_arguments('-static', language: 'cpp')
endif

cxxid = cxx.get_id()

if cxxid == 'clang' and opt_libcxx
    add_project_arguments('-stdlib=libc++', language: 'cpp')
    add_project_link_arguments('-stdlib=libc++', language: 'cpp')
    libcxx = cxx.find_library('c++', required: true, static: opt_staticstdlib or opt_static)
    libcxxabi = cxx.find_library('c++abi', required: true, static: opt_staticstdlib or opt_static)
    deps += libcxx
    deps += libcxxabi
    if opt_staticstdlib
        add_project_link_arguments('-static-libgcc', language: 'cpp')
    endif
endif

if cxxid == 'gcc' and opt_staticstdlib
    add_project_link_arguments('-static-libgcc', language: 'cpp')
    add_project_link_arguments('-static-libstdc++', language: 'cpp')
endif

textdomain = 'org.codeberg.wreedb.confidant'
config_hpp = configuration_data()
config_hpp.set_quoted('GETTEXTDOMAIN', textdomain)
config_hpp.set_quoted('PROJECT_VERSION', meson.project_version())
config_hpp.set_quoted('PROJECT_NAME', meson.project_name())
config_hpp.set_quoted('LOCALEDIR', get_option('prefix') / get_option('localedir'))
configure_file(output: 'config.hpp', configuration: config_hpp)

subdir('i18n')
subdir('misc/completions')
subdir('doc/man')

libucl_dep = dependency(
    'libucl',
    required: false,
    fallback: ['libucl', 'libucl_dep'],
    static: opt_static or get_option('static-libucl'))

lyra_dep = dependency(
    'lyra',
    required: false,
    fallback: ['lyra', 'lyra_dep'])

sources = files(
    'src/util.cpp',
    'src/fmt.cpp',
    'src/xdg.cpp',
    'src/help.cpp',
    'src/parse.cpp',
    'src/settings/local.cpp',
    'src/settings/global.cpp',
    'src/actions/get.cpp',
    'src/actions/link.cpp',
    'src/actions/dump.cpp'
)

deps += libucl_dep
deps += lyra_dep

confidant_lib = static_library(
    'confidant',
    sources,
    include_directories: ['src'],
    dependencies: deps
)
subdir('test')

executable(
  'confidant',
  'src/main.cpp',
  include_directories: ['src'],
  install: true,
  dependencies: deps,
  link_with: confidant_lib)