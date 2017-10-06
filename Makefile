PROJECT = autodim

CC = gcc
CFLAGS = -I $(INCDIR) -Wall
TFLAGS = -Wl,-eautodim

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
	gcc -o $@ $^ $(CFLAGS)

run: $(PROJECT)
	./$(PROJECT)

# Compile into process instead of daemon
test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(TFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJDIR)/*.o
	rm -f $(PROJECT)
