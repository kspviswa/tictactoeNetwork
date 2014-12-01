CC=g++
CFLAGS= -g -pthread -lprotobuf


all: client.out

client.out: tictac.o transport.o  packetProcess.o game_logic.o
	$(CC) $(CFLAGS) tictac.o transport.o game_logic.o packetProcess.o -lprotobuf -o client.out

game_logic.o: game_logic.cpp
	$(CC) $(CFLAGS) -c game_logic.cpp -o game_logic.o

transport.o: transport.cpp
	$(CC) $(CFLAGS) -c transport.cpp -o transport.o

tictac.o: tictac.cpp
	$(CC) $(CFLAGS) -lprotobuf -c tictac.cpp -o tictac.o

packetProcess.o: packetProcess.cpp
	$(CC) $(CFLAGS) -lprotobuf -c packetProcess.cpp -o packetProcess.o


clean:
	rm -rf *o client 
