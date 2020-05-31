.PHONY: all getwords

all:
	gcc -g3 -std=c99 -pedantic -Wall sator.c -o sator

getwords:
	gcc -g3 -std=c99 -pedantic -Wall getwords.c -o getwords