#Dawid Wegner (279743)
BIN = transport
OBJ = transport.o streamReader.o streamInfo.o streamManager.o dataPacket.o frameEntry.o
FLAGS = -std=c++11 -c -Wall -Wextra

transport: $(OBJ)
	g++ -std=c++11 -o transport $(OBJ)

transport.o: transport.cpp
	g++ $(FLAGS) transport.cpp

streamReader.o: streamReader.cpp
	g++ $(FLAGS) streamReader.cpp

streamInfo.o: streamInfo.cpp
	g++ $(FLAGS) streamInfo.cpp

streamManager.o: streamManager.cpp
	g++ $(FLAGS) streamManager.cpp

dataPacket.o: dataPacket.cpp
	g++ $(FLAGS) dataPacket.cpp

frameEntry.o: frameEntry.cpp
	g++ $(FLAGS) frameEntry.cpp

clean:
	@rm -vf *.o

distclean:
	@rm -vf *.o $(BIN)
