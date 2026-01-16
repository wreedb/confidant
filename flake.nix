# SPDX-FileCopyrightText: 2026 Will Reed
# SPDX-License-Identifier: GPL-3.0-or-later

{
  description = "Your configuration pal and confidant";

  inputs.nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let

        pkgs = nixpkgs.legacyPackages.${system};
        lib = nixpkgs.legacyPackages.${system}.lib;
        stdenv = pkgs.gcc15Stdenv;
        version = builtins.readFile ./.version;

        nativeBuildInputs = with pkgs; [
          scdoc
          pkg-config
          ninja
          meson
          gettext
        ];

        buildInputs = with pkgs; [
          libucl
          lyra
        ];

        confidant-package = stdenv.mkDerivation {
          pname = "confidant";
          inherit version;
          src = ./.;

          meta = {
            description = "Your configuration pal and confidant";
            long-description = ''
              Confidant is a configuration (or dotfile) management tool, which combines the
              practical symlink-based approach of tools like GNU Stow with simple (but powerful)
              templating syntax inspired by Ansible. The result is an easy to use, fast and reliable
              dotfile management workflow.
            '';
            homepage = "https://wreedb.github.io/confidant";
            license = lib.licenses.gpl3Plus;
            mainProgram = "confidant";
          };

          inherit nativeBuildInputs buildInputs;

          mesonBuildType = "release";
          mesonFlags = [
            "-D man-pages=enabled"
            "-D strip=true"
          ];

        };

      in {
        packages.confidant = confidant-package;

        apps.confidant = {
          type = "app";
          program = "${confidant-package}/bin/confidant";
        };

        apps.default = self.apps.${system}.confidant;
        packages.default = confidant-package;

        devShells.default = pkgs.mkShell {
          inherit nativeBuildInputs buildInputs;
        };
      }
    );
}
