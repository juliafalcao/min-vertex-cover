all: main

main: main.cpp utils graph heuristics
	g++ -g main.cpp -o main utils graph heuristics

utils: utils.cpp
	g++ -g -c utils.cpp -o utils

graph: graph.cpp
	g++ -g -c graph.cpp -o graph

heuristics: heuristics.cpp
	g++ -g -c heuristics.cpp -o heuristics