# Astrum

A test game library for C++. Mainly made to experiment with SDL. The API is largely based on Love2D, and is also fairly incomplete (notably, there's no audio API yet, shearing and a couple configuration options are silently ignored, and several functions are included in header files but not implemented anywhere).

I'm not planning on doing anything with the code anymore. The library is usable for small programs (without audio, for one thing); the `examples` directory includes several test programs I wrote, and you can find those online (compiled via Emscripten) [here](https://mwpuppire.github.io/astrum).

## Building

Requires SDL2, SDL_ttf, SDL_image, SDL_gfx, and OpenGL. I haven't tried building it on any systems other than my own, so if CMake can't find the packages on your computer, you'll have to figure it out.

Documentation exists for a few classes (not many, though), and can be generated with Doxygen.

It can run on the web via Emscripten if you have that installed.
