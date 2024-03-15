# Asteroids Game 🌠

Simple game made using [raylib](https://github.com/raysan5/raylib) written in C.

### Some features:
- Dynamic difficulty
- Music & Sound effects
- Particles

[game_preview.webm](https://github.com/LauP31/AsteroidsGame/assets/36768564/401e1202-e854-438b-a60a-6117ae7a39e4)

## Build

Requirements: [raylib](https://github.com/raysan5/raylib)

Windows:\
`gcc -o game.exe src/*.h src/*.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -O1 -I./include -L./lib -lraylib -lopengl32 -lgdi32 -lwinmm -mwindows -DPLATFORM_DESKTOP`

Linux:
`gcc -o game src/*.h src/*.c -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s -O1 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -DPLATFORM_DESKTOP`
