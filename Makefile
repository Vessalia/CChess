CC = clang
CFLAGS = -Wall -Wpedantic -Wextra -std=c11
DEBUG = -g

INCLUDE_FLAGS = -Iinclude -I/usr/include/SDL2 -I/usr/include/SDL_image
LINKER_FLAGS = -lSDL2 -lSDL2_image

SRCS = $(wildcard src/*.c)
OBJDIR = objects
OBJS = $(patsubst src/%.c,$(OBJDIR)/%.o,$(SRCS))

ALL: main

main: $(OBJDIR) $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG) $(LINKER_FLAGS) $(OBJS) -o main

$(OBJS): $(OBJDIR)/%.o: src/%.c $(HDR) $(OBJDIR)
	$(CC) $(CFLAGS) $(DEBUG) -c $< $(INCLUDE_FLAGS) -o $@

$(OBJDIR):
	mkdir $(OBJDIR)

clean:
	rm -fr $(OBJS) $(OBJDIR) main
