project('libucl', 'c', 
        meson_version: '>= 1.1',
        version: '0.9.2',
        default_options: [
            'default_library=static',
            'strip=true',
            'warning_level=2',
            'c_args=-Wno-pointer-sign'
        ])

incdirs = ['src', 'include', 'klib', 'uthash']

libucl_sources = files(
    'src/ucl_emitter.c',
    'src/ucl_emitter_streamline.c',
    'src/ucl_emitter_utils.c',
    'src/ucl_hash.c',
    'src/ucl_msgpack.c',
    'src/ucl_parser.c',
    'src/ucl_schema.c',
    'src/ucl_sexp.c',
    'src/ucl_util.c',
)

libucl = static_library(
    'ucl',
    libucl_sources,
    include_directories: incdirs)

libucl_dep = declare_dependency(
    link_with: libucl,
    include_directories: incdirs)