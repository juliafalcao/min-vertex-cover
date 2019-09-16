
all: main

main: main.o utils.o graph.o
	g++ main.cpp -o main.o utils.o graph.o

utils: utils.o
	$(CC) -c utils.cpp -o utils.o

graph: graph.o
	$(CC) -c graph.cpp -o graph.o

clean:
	rm *.o