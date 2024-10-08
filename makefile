all: program

program: main.o stack.o StackError.o HashProtection.o CanaryProtection.o
	g++ main.o stack.o StackError.o HashProtection.o CanaryProtection.o

main.o: main.cpp
	g++ -c main.cpp

stack.o: stack.cpp stack.h
	g++ -c stack.cpp stack.h

StackError.o: StackError.h
	g++ StackError.h

HashProtection.o: HashProtection.cpp HashProtection.h
	g++ -c HashProtection.cpp HashProtection.h

CanaryProtection.o: CanaryProtection.cpp CanaryProtection.h
	g++ -c CanaryProtection.cpp CanaryProtection.h

