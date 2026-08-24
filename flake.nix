{
  description = "Bounce balls";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      # System helper (supports x86_64-linux, aarch64-darwin, etc.)
      supportedSystems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      forAllSystems = nixpkgs.lib.genAttrs supportedSystems;
    in
    {
      devShells = forAllSystems (system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        {
          default = pkgs.mkShell {
            # Build tools available in the shell
            nativeBuildInputs = with pkgs; [
              gcc           # Compiler (or clang)
              cmake         # Build system
              pkg-config    # Helps compiler find raylib headers/libs automatically
            ];

            # Libraries your project links against
            buildInputs = with pkgs; [
              raylib
            ];

            # Custom shell hook (runs when entering environment)
            shellHook = ''
              echo "Raylib loaded."
              echo "Compiler: $(gcc --version | head -n 1)"
            '';
          };
        });
    };
}
