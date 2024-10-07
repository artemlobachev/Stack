all: program

program: main.o stack.o StackError.o HashFunction.o CanaryProtection.o
	g++ main.o stack.o StackError.o HashFunction.o CanaryProtection.o

main.o: main.cpp
	g++ -c main.cpp

stack.o: stack.cpp stack.h
	g++ -c stack.cpp stack.h

StackError.o: StackError.cpp StackError.h
	g++ -c StackError.cpp StackError.h

HashFunction.o: HashFunction.cpp HashFunction.h
	g++ -c HashFunction.cpp HashFunction.h

CanaryProtection.o: CanaryProtection.cpp CanaryProtection.h
	g++ -c CanaryProtection.cpp CanaryProtection.h
