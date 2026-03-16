CFLAGS=-I./src/include -g
CLIBS=-lGL -lX11 -lXrandr -lm -lglfw
OUTPUT=game.elf

OBJECTS=$(subst src/engine/,obj/eng_,$(patsubst %.c,%.o,$(shell find src/engine/ -name "*.c"))) $(subst src/game/,obj/gam_,$(patsubst %.c,%.o,$(shell find src/game/ -name "*.c"))) obj/ma.o

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(CLIBS)

obj/eng_%.o: src/engine/%.c src/include/* obj/.gitignore # GNUMake is crazy
	$(CC) $(CFLAGS) -c -o $@ $<
obj/gam_%.o: src/game/%.c src/include/* obj/.gitignore # GNUMake is crazy
	$(CC) $(CFLAGS) -c -o $@ $<
obj/ma.o: src/thirdparty/miniaudio.h
	$(CC) $(CFLAGS) -x c -D MINIAUDIO_IMPLEMENTATION -c -o $@ $<
obj/.gitignore:
	mkdir -p obj
	echo * > obj/.gitignore
