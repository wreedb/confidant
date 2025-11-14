# SPDX-FileCopyrightText: 2025 Will Reed
#
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

        nativeBuildInputs = with pkgs; [
          pkg-config
          ninja
          meson
          cmake
        ];

        buildInputs = with pkgs; [
          libucl
          cli11
        ];

      in {
        packages.default = stdenv.mkDerivation {
          pname = "confidant";
          version = "0.1.0";
          src = ./.;

          meta = {
            description = "Your configuration pal and confidant";
            homepage = "https://codeberg.org/wreedb/confidant";
            license = lib.licenses.gpl3Plus;
          };

          inherit nativeBuildInputs buildInputs;

          mesonBuildType = "release";
          mesonFlags = ["-D strip=true"];

        };

        devShells.default = pkgs.mkShell {
          inherit nativeBuildInputs buildInputs;
        };
      }
    );
}