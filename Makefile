DIRINCLUDE=./include
DIROBJETO=./obj
CC=gcc
CFLAGS=-I$(DIRINCLUDE) -g

_DEPEND = arq_reg.h
DEPEND = $(patsubst %,$(DIRINCLUDE)/%,$(_DEPEND))

_OBJETO = arq_reg.o main.o
OBJETO = $(patsubst %,$(DIROBJETO)/%,$(_OBJETO))

$(DIROBJETO)/%.o: %.c $(DEPEND)
	$(CC) -c -o $@ $< $(CFLAGS)

gerenciador_arq_led.out: $(OBJETO)
	$(CC) -o ./bin/$@ $^ $(CFLAGS)
