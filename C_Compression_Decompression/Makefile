CFLAGS = -Wall -Wextra -Wpedantic -g
CC = gcc $(CFLAGS)
OBJECTS1 = decode.o stack.o huffman.o
OBJECTS2 = encode.o huffman.o queue.o is.o stack.o bitV.o
.PHONY: encode decode

all: encode decode

decode: $(OBJECTS1)
	$(CC) $(OBJECTS1) -o decode
	
encode: $(OBJECTS2)
	$(CC) $(OBJECTS2) -o encode

encode.o: encode.c
	$(CC) -c encode.c

decode.o: decode.c
	$(CC) -c decode.c

stack.o: stack.c
	$(CC) -c stack.c

queue.o: queue.c
	$(CC) -c queue.c

huffman.o: huffman.c
	$(CC) -c huffman.c

is.o: is.c
	$(CC) -c is.c

bitV.o: bitV.c
	$(CC) -c bitV.c
clean:
	rm -f *.o decode encode

