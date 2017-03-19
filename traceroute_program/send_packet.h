//Dawid Wegner (279743)
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

struct icmphdr create_icmp_header(int pid, int seq);
void send_packet(int fd, char ip_addr[], struct icmphdr* icmp_header, int ttl);

