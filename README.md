# 3DRenderer

A 3D graphics pipeline entirely on CPU. This implements a complete graphical pipeline without the assistance of a GPU. It supports reading models from Wavefront .obj files, too.

This project is done being developed-- it's mostly just for fun, anyway. I've written basic instructions for compiling this project below, but I also published binaries for Linux and macOS in the final release.

## Compiling

Below will work for Linux and macOS. If you're on Windows, you're on your own. WSL2 may be of assistance. You'll need SDL2, your favorite C compiler (GCC or Clang recommended), and `make`.

1. Install SDL2. This is required for creating a display window. On Linux, your distro of choice should ship a version in the system package manager, probably named `sdl`, or `sdl2`, or something similar. On macOS, you can install from Homebrew with `brew install sdl2`. Note that, now that SDL3 is released, many packagers have renamed their SDL2 packages to something like `sdl2-compat`.

2. Ensure make is installed. Open up a terminal and run `make` to see if it is. It should be installed by default on any Linux distro; macOS might prompt you to install developer tools.

3. Clone this repo onto your computer.

4. CD into the root project directory. Check the Makefile line 14 for `RUNTIME ?= ...` and make sure it is set to your correct platform-- either `linux` or `macos`. There is also support in the Makefile for `windows`, but I can't be sure it will work.

5. Run `make` to compile, then `make publish` to create a neatly packaged binary.

6. CD into `publish` and run `./3drender <model.obj>`, where `<model.obj>` is a Wavefront object file. You can use `../cube.obj` or `../3d.obj` to use one of the models I included.

## Running a Pre-Compiled Version

You should hopefully find compiled binaries of the latest version of the codebase on the latest GitHub release. Again, only for Linux and macOS; Windows users look above, sorry. Simply download the binary for your OS and enjoy. Run it from a terminal with `./3drender <model.obj>` where `<model.obj>` is a Wavefront object file. You can export models from Blender as Wavefront objects, or you can download one of the two that I included in this repository: `cube.obj` and `3d.obj`.
