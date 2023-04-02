CC = clang
CFLAGS = -Wall -Wpedantic -Wextra -std=c11
DEBUG = -g

INCLUDE_FLAGS = -Iinclude -I/usr/include/SDL2
LINKER_FLAGS = -lSDL2
SRCS = src/Entry.c src/Board.c
OBJDIR = objects

OBJS = $(OBJDIR)/Board.o $(OBJDIR)/Entry.o

ALL: main

main: $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $(LINKER_FLAGS) $(OBJS) -o main

$(OBJDIR)/Board.o: src/Board.c $(HDR) $(OBJDIR)
	$(CC) $(CFLAGS) $(DEBUG) -c src/Board.c $(INCLUDE_FLAGS) -o $(OBJDIR)/Board.o

$(OBJDIR)/Entry.o: src/Entry.c $(HDR) $(OBJDIR)
	$(CC) $(CFLAGS) $(DEBUG) -c src/Entry.c $(INCLUDE_FLAGS) -o $(OBJDIR)/Entry.o

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -fr $(OBJS) $(OBJDIR) main