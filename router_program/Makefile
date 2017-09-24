#Dawid Wegner (279743)
BIN = router
OBJ = router.o connection.o directConnection.o packetManager.o routerPacket.o
FLAGS = -std=c++11 -c -Wall -Wextra

router: $(OBJ)
	g++ -std=c++11 -o router $(OBJ)

router.o: router.cpp
	g++ $(FLAGS) router.cpp

connection.o: connection.cpp
	g++ $(FLAGS) connection.cpp

directConnection.o: directConnection.cpp
	g++ $(FLAGS) directConnection.cpp

packetManager.o: packetManager.cpp
	g++ $(FLAGS) packetManager.cpp

routerPacket.o: routerPacket.cpp
	g++ $(FLAGS) routerPacket.cpp

clean:
	@rm -vf *.o $(BIN)
