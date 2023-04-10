CFLAGS = -ansi -pedantic-errors -Wall -Wextra -Werror -O2
LFLAGS = -lm -static -s
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)
BIN = crud

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) -MMD -MP -c $< $(CFLAGS)

.PHONY : clean
clean:
	$(RM) $(BIN) $(OBJ) $(DEP) *~

-include $(DEP)
