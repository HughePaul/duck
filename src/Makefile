CC = gcc
RM = /bin/rm -f

CFLAGS = -O3 -Wall -pedantic

LINKER = -lm

OBJECTS  = main.o token.o stack.o data.o load.o pass.o \
        run.o keywords.o expr.o std.o
INCLUDES = main.h token.h stack.h data.h load.h pass.h \
        run.h keywords.h expr.h std.h

DEST = ../duck

build: duck

clean:
	$(RM) $(OBJECTS) $(DEST) duck.o core

.c.o:
	$(CC) $(CFLAGS) -c $<

duck: duck.c $(OBJECTS)
	$(CC) $(CFLAGS) -c duck.c $(LINKER)
	$(CC) $(CFLAGS) -o $(DEST) duck.o $(OBJECTS) $(LINKER)

main.o:		main.c		$(INCLUDES)
token.o:	token.c		$(INCLUDES)
stack.o:	stack.c		$(INCLUDES)
data.o:		data.c		$(INCLUDES)
load.o:		load.c		$(INCLUDES)
pass.o:		pass.c		$(INCLUDES)
run.o:		run.c		$(INCLUDES)
expr.o:		expr.c		$(INCLUDES)
keywords.o:	keywords.c	$(INCLUDES)
std.o:		std.c		$(INCLUDES)

