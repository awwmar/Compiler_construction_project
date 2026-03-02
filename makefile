# makefile - Group No: 38
# Members: Amar Kumar(2023A7PS0518P), Krishna Saraf(2023A7PS0606P), Ritik Kumar(2023A7PS0553P)
#          Shivansh Saxena(2023A7PS0637P), Hitashi Raizada(2023A7PS0500P), Arnav Sahoo(2023A7PS0560P)
CC = gcc
CFLAGS = -Wall -g

stage1exe: driver.o lexer.o parser.o
	$(CC) $(CFLAGS) -o stage1exe driver.o lexer.o parser.o

driver.o: driver.c lexer.h lexerDef.h parser.h parserDef.h
	$(CC) $(CFLAGS) -c driver.c -o driver.o

lexer.o: lexer.c lexer.h lexerDef.h
	$(CC) $(CFLAGS) -c lexer.c -o lexer.o

parser.o: parser.c parser.h parserDef.h lexer.h lexerDef.h
	$(CC) $(CFLAGS) -c parser.c -o parser.o

clean:
	rm -f *.o stage1exe cleanOutput.txt
