# AsteroidsSDL3
Asteroids 2D using vector graphics and wireframe model rendering with C++ and SDL3

```powershell
g++ -Iinclude -Llib src/*.cpp -lSDL3 -lSDL3_ttf -o asteroids
```

<br><br><br><br>
![Screenshot](screenshot.png)

<br><br><br><br>
Emscripten build: <br>

Use main.cpp from emscription folder <br>
Get a copy of the SDL source: <br>
git clone https://github.com/libsdl-org/SDL.git vendored/SDL <br>
git clone https://github.com/libsdl-org/SDL_ttf.git vendored/SDL_ttf <br>
Add emscripten to include folder <br>
Install emsdk <br>
Run in emcmdprompt: <br>
```powershell
emcmake cmake -S . -B build -DSDLTTF_VENDORED=ON -DSDLTTF_HARFBUZZ=OFF
cd build
emmake make
```
