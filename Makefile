CC = clang
CFLAGS = -ansi -pedantic-errors -g
LFLAGS =

crud: main.o hash.o
	$(CC) $(LFLAGS) $^ -o $@

main.o: main.c hash.h
	$(CC) $(CFLAGS) $< -c

hash.o: hash.c hash.h
	$(CC) $(CFLAGS) $< -c

.PHONY : clean
clean:
	$(RM) crud *.o *~
