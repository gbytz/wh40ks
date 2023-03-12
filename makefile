run: linux
	./wh40ks

linux: main.c
	gcc main.c -Wall -lSDL2 -lSDL2_gfx -o wh40ks

osx:
	gcc -I/opt/homebrew/include main.c -L/opt/homebrew/lib -lSDL2 -lSDL2_gfx -o wh40ks
