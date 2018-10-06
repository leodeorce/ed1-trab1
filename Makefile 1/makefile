### Makefile ###

all: prog

prog: main.o netmap.o roteador.o terminal.o
	gcc -o prog main.o netmap.o roteador.o terminal.o

terminal.o: terminal.c
	gcc -c terminal.c

roteador.o: roteador.c
	gcc -c roteador.c

netmap.o: netmap.c
	gcc -c netmap.c

main.o: main.c
	gcc -c main.c

clean:
	rm -rf *.o

rmproper: clean
	rm -rf prog

run: prog
	./prog entrada.txt
