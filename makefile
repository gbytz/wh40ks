run: build
	./wh40ks

build: main.c
	gcc main.c -lSDL2 -lSDL2_gfx -o wh40ks
