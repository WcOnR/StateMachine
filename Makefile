COMPILER = g++
FLAGS = -Wall -std=c++11

SM = StateMachine
I = Interfaces

all: main.o $(SM).o $(I).o
	$(COMPILER) -D DEBUG $(FLAGS) main.o $(SM).o $(I).o

main.o: main.cpp $(SM).hpp
	$(COMPILER) $(FLAGS) main.cpp -c

$(SM).o: $(SM).cpp $(SM).hpp $(I).hpp
	$(COMPILER) $(FLAGS) $(SM).cpp -c

$(I).o: $(I).cpp $(I).hpp
	$(COMPILER) $(FLAGS) $(I).cpp -c