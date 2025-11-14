[group("flake")]
build-flake:
    nix build

[group("flake")]
clean-flake:
    -rm result

[group("git")]
commit:
    convco commit -i