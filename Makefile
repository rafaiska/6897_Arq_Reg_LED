DIRINCLUDE=./include
DIROBJETO=./obj
CC=gcc
CFLAGS=-I$(DIRINCLUDE)

_DEPEND = tad.h instrucao.h
DEPEND = $(patsubst %,$(DIRINCLUDE)/%,$(_DEPEND))

_OBJETO = tad.o main.o instrucao.o
OBJETO = $(patsubst %,$(DIROBJETO)/%,$(_OBJETO))

$(DIROBJETO)/%.o: %.c $(DEPEND)
	$(CC) -c -o $@ $< $(CFLAGS)

gerenciador_arq_led.out: $(OBJETO)
	$(CC) -o ./bin/$@ $^ $(CFLAGS)
