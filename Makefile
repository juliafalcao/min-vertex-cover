
all: main

main: main.o utils.o graph.o heuristics.o
	g++ -g main.cpp -o main.o utils.o graph.o heuristics.o

utils: utils.o
	g++ -g -c utils.cpp -o utils.o

graph: graph.o
	g++ -g -c graph.cpp -o graph.o

heuristics: heuristics.o
	g++ -g -c heuristics.cpp -o heuristics.o

clean:
	rm *.o