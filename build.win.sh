set -e

#  Linux
# CC=cc
# CFLAGS=
# CLIBS="-lGL -lX11 -lXrandr -lm -I./include"
# OUTPUT=game.elf

#  MS Windows
CC=x86_64-w64-mingw32-gcc
CFLAGS=
CLIBS="-lopengl32 -lgdi32 -lm -I./include"
OUTPUT=game.exe

ENGINE_SRC=engine/*.c
GAME_SRC=game/*.c

set -x

mkdir -p obj
echo "*" > obj/.gitignore

$CC $CFLAGS -x c -D RGFW_IMPLEMENTATION -D RGFW_OPENGL -D RGFWDEF=extern thirdparty/RGFW.h -c -o obj/RGFW.lib $CLIBS
$CC $CFLAGS -x c -D MINIAUDIO_IMPLEMENTATION thirdparty/miniaudio.h -c -o obj/ma.lib $CLIBS
$CC $CFLAGS $ENGINE_SRC $GAME_SRC obj/RGFW.lib obj/ma.lib -o $OUTPUT $CLIBS
