# We prefer unstable packages as those are typically newer
with import <unstable> {};

stdenv.mkDerivation {
  name = "env";
  nativeBuildInputs = [ clang_11 clang-tools cmake ];
  