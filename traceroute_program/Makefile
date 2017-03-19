#Dawid Wegner (279743)
BIN = traceroute
OBJ = traceroute.o recv_packet.o utils.o send_packet.o

traceroute: $(OBJ)
	gcc -std=gnu99 -o traceroute $(OBJ)

recv_packet.o: recv_packet.c
	gcc -std=gnu99 -c -Wall -Wextra recv_packet.c

traceroute.o: traceroute.c
	gcc -std=gnu99 -c -Wall -Wextra traceroute.c

utils.o: utils.c
	gcc -std=gnu99 -c -Wall -Wextra utils.c

send_packet.o: send_packet.c
	gcc -std=gnu99 -c -Wall -Wextra send_packet.c

clean:
	@rm -vf *.o $(BIN)
