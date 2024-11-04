



all: dinoGame.c
	gcc dinoGame.c -o run -l ncurses

clear:
	rm -f run