CC=g++
CFLAGS= -g -Wall -pthread -lprotobuf


all: client.out

client.out: tictac.o game_logic.o  game_util.o
	$(CC) $(CFLAGS) tictac.o game_logic.o  game_util.o -lprotobuf -o client.out

game_logic.o: game_logic.cpp
	$(CC) $(CFLAGS) -c game_logic.cpp -o game_logic.o

game_util.o: game_util.cpp
	$(CC) $(CFLAGS) -c game_util.cpp -o game_util.o

tictac.o: tictac.cpp
	$(CC) $(CFLAGS) -lprotobuf -c tictac.cpp -o tictac.o

clean:
	rm -rf *o client 
