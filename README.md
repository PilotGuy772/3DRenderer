# 3DRenderer

A 3D graphics pipeline entirely on CPU. This implements a complete graphical pipeline without the assistance of a GPU. It supports reading models from Waveform .obj files, too.

This project is currently not fleshed out enough to simply download and run; you will have to compile it yourself. Luckily, it's pretty simple. You'll need your favorite C compiler, SDL2, and make.

## Compiling

Below will work for Linux and macOS. If you're on Windows, you're on your own. WSL2 may be of assistance.

1. Install SDL2. This is required for creating a display window. On Linux, your distro of choice should ship a version in the system package manager, probably named `sdl`, or `sdl2`, or something similar. On macOS, you can install from Homebrew with `brew install sdl2`.

2. Ensure make is installed. Open up a terminal and run `make` to see if it is. It should be installed by default on any Linux distro; macOS might prompt you to install developer tools.

3. Clone this repo onto your computer.

4. CD into the root project directory.

5. Run `make` to compile, then `make publish` to create a neatly packaged binary.

6. CD into `publish` and run `./3drender <model.obj>`, where `<model.obj>` is a Wavefront object file. You can use `../cube.obj` or `../3d.obj` to use one of the models I included.

## Running a Pre-Compiled Version

You should hopefully find compiled binaries of the latest version of the codebase on the latest GitHub release. Again, only for Linux and macOS; Windows users look above, sorry. Simply download the binary for your OS and enjoy. Run it from a terminal with `./3drender <model.obj>` where `<model.obj>` is a Wavefront object file. You can export models from Blender as Wavefront objects, or you can download one of the two that I included in this repository: `cube.obj` and `3d.obj`.