all: main

main: main.cpp utils graph heuristics genetic
	g++ -g main.cpp -o main utils graph heuristics genetic

utils: utils.cpp
	g++ -g -c utils.cpp -o utils

graph: graph.cpp
	g++ -g -c graph.cpp -o graph

heuristics: heuristics.cpp
	g++ -g -c heuristics.cpp -o heuristics

genetic: genetic.cpp
	g++ -g -c genetic.cpp -o genetic