### Makefile ###

CC = gcc

CFLAGS = -Wall

FONTES = $(wildcard *.c)

OBJETOS = $(FONTES:.c=.o)

ENTRADA = entrada.txt

EXECUTAVEL = prog

all: $(EXECUTAVEL)

$(EXECUTAVEL): $(OBJETOS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) $^

clean:
	rm -rf *.o

rmproper: clean
	rm -rf $(EXECUTAVEL)

run: $(EXECUTAVEL)
	./$(EXECUTAVEL) $(ENTRADA)
