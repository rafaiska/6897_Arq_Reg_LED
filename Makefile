DIRINCLUDE=./include
DIROBJETO=./obj
CC=gcc
CFLAGS=-I$(DIRINCLUDE) -g

_DEPEND = arq_reg.h arvore_b.h
DEPEND = $(patsubst %,$(DIRINCLUDE)/%,$(_DEPEND))

_OBJETO = arq_reg.o main.o arvore_b.o
OBJETO = $(patsubst %,$(DIROBJETO)/%,$(_OBJETO))

$(DIROBJETO)/%.o: %.c $(DEPEND)
	@mkdir -p obj
	$(CC) -c -o $@ $< $(CFLAGS)

gerenciador_arq_led.out: $(OBJETO)
	@mkdir -p bin
	$(CC) -o ./bin/$@ $^ $(CFLAGS)
