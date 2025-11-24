
version=$(cat .version)
repo=$PWD

sourcefiles=(
    .version
    .editorconfig
    REUSE.toml
    cliff.toml
    CHANGELOG.md
    README.md
    AUTHORS.md
    THANKS.md
    CONTRIBUTING.md
    INSTALL.md
    LICENSE.md
    mkdocs.yml
    meson.build
    meson.options
    flake.nix
    flake.lock
    subprojects/libucl.wrap
    subprojects/lyra.wrap
    subprojects/packagefiles/libucl/meson.build
    subprojects/packagefiles/lyra/meson.build
)

sourcedirs=(
    doc
    src
    include
    misc
    test
    LICENSES
    example
)

mkdir -p rel
pushd rel

    mkdir -p binaries source manuals completions

    for n in ${sourcefiles[@]}; do
        install -Dvm 0644 ${repo}/${n} source/${n}
    done
    
    for n in ${sourcefiles[@]}; do
        install -Dvm 0644 ${repo}/${n} source/${n}
    done

    for d in ${sourcedirs[@]}; do
        cp -a ${repo}/${d} source/${d}
    done

    mv source confidant-${version}

    tar --no-same-owner --no-xattrs --no-acls --no-selinux --format=pax -f confidant-${version}.source.tar -vc confidant-${version}
    rm -rf confidant-${version}

popd