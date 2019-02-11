CFLAGS = -Wall -Wextra -Werror -Wpedantic -g
CC = gcc $(CFLAGS)
SPECIAL = -Werror -Wpedantic -g
CCSPEC = gcc $(SPECIAL)

.PHONY: all
all	:	banhammer

banhammer	:	banhammer.o hash.o ll.o aes.o bf.h hash.h aes.h ll.h lex.yy.o
	$(CC) -o banhammer banhammer.o hash.o ll.o aes.o lex.yy.o -lfl

banhammer.o	:	banhammer.c
	$(CC) -c banhammer.c -lfl

hash.o	:	hash.c
	$(CC) -c hash.c

ll.o	:	ll.c
	$(CC) -c ll.c

aes.o	:	aes.c
	$(CC) -c aes.c

lex.yy.o	:	lex.yy.c
	$(CCSPEC) -c lex.yy.c -lfl

lex.yy.c	:	words.l
	flex words.l

.PHONY: clean
clean	:
	rm -f *.o banhammer lex.yy.c
