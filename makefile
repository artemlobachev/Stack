all: program

program: main.o stack.o StackFunc.o StackError.o
	g++ main.o stack.o StackFunc.o StackError.o

main.o: main.cpp
	g++ -c main.cpp

stack.o: stack.cpp stack.h
	g++ -c stack.cpp stack.h

StackFunc.o: StackFunc.cpp StackFunc.h
	g++ -c StackFunc.cpp StackFunc.h

StackError.o: StackError.cpp StackError.h
	g++ -c StackError.cpp StackError.h


