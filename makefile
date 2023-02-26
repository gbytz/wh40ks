run: build
	./wh40ks

build: main.c
	gcc main.c -lSDL2 -o wh40ks
