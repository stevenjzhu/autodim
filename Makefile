PROJECT = autodim

CC = gcc
CFLAGS = -I $(INCDIR) -Wall

SRCDIR = src
OBJDIR = build
INCDIR = include

_DEPS = autodim.h
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

_OBJ = main.o autodim.o
OBJ = $(patsubst %,$(OBJDIR)/%,$(_OBJ))

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROJECT): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

run: $(PROJECT)
	./$(PROJECT)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(PROJECT)
