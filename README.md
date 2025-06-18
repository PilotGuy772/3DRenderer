# 3DRenderer

A 3D graphics pipeline entirely on CPU. This implements a complete graphical pipeline without the assistance of a GPU. It supports reading models from Waveform .obj files, too.

This project is currently not fleshed out enough to simply download and run; you will have to compile it yourself. Luckily, it's pretty simple. You'll need your favorite C compiler, SDL2, and make.

## Compiling

Below will work for Linux and macOS. If you're on Windows, you're on your own. WSL2 may be of assistance.

1. Install SDL2. This is required for creating a display window. On Linux, your distro of choice should ship a version in the system package manager, probably named `sdl`, or `sdl2`, or something similar. On macOS, you can isntall from Homebrew with `brew install sdl2`.

2. Ensure make is installed. Open up a terminal and run `make` to see if it is. It should be installed by default on any Linux distro; macOS might prompt you to install developer tools.

3. Clone this repo onto your computer.

4. CD into the root project directory.

5. Run `make` to compile and `make run` to run! It works on my machine, should work on yours too. Hopfully.