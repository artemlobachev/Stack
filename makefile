all: program

DEBUG: main.o stack.o StackError.o HashFunction.o
	g++ main.o stack.o HashFunction.o -D HASH -D CANARY1 -D DEBUG

HASH: main.o stack.o StackError.o HashFunction.o
	g++ main.o stack.o HashFunction.o -D HASH

CANARY: main.o stack.o StackError.o HashFunction.o
	g++ main.o stack.o HashFunction.o -D CANARY1

program: main.o stack.o StackError.o HashFunction.o
	g++ main.o stack.o HashFunction.o

main.o: main.cpp
	g++ -c main.cpp

stack.o: stack.cpp stack.h
	g++ -c stack.cpp stack.h

StackError.o: StackError.h
	g++ StackError.h

HashProtection.o: HashFunction.cpp HashFunction.h
	g++ -c HashFunction.cpp HashFunction.h
