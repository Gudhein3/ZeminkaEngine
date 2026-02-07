CFLAGS=-I./include -g
CLIBS=-lGL -lX11 -lXrandr -lm
OUTPUT=game.elf

OBJECTS=$(subst engine/,obj/eng_,$(patsubst %.c,%.o,$(shell find engine/ -name "*.c"))) $(subst game/,obj/gam_,$(patsubst %.c,%.o,$(shell find game/ -name "*.c"))) obj/ma.o obj/RGFW.o

$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(CLIBS)

obj/eng_%.o: engine/%.c include/* obj/.gitignore # GNUMake is crazy
	$(CC) $(CFLAGS) -c -o $@ $<
obj/gam_%.o: game/%.c include/* obj/.gitignore # GNUMake is crazy
	$(CC) $(CFLAGS) -c -o $@ $<
obj/ma.o: thirdparty/miniaudio.h
	$(CC) $(CFLAGS) -x c -D MINIAUDIO_IMPLEMENTATION -c -o $@ $<
obj/RGFW.o: thirdparty/RGFW.h
	$(CC) $(CFLAGS) -x c -D RGFW_IMPLEMENTATION -D RGFWDEF=extern -D RGFW_OPENGL -c -o $@ $<
obj/.gitignore:
	mkdir -p obj
	echo * > obj/.gitignore
