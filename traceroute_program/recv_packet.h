//Dawid Wegner (279743)
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int sock_fd;
  fd_set descs;
  struct timeval t_limit;
} recv_config;

typedef struct {
  char snd_ip[20];
  struct iphdr* ip_header;
  struct icmp* packet_data;
  int r_time;
} recv_packet;

recv_config get_recv_config(int sock_fd, int usec_limit);
int wait_for_packets(recv_config* cfg);
int receive_packet(int fd, u_int8_t buff[], recv_packet* packet);
int get_pckt_pid(recv_packet* packet);
int get_pckt_seq(recv_packet* packet);
int get_icmp_type(recv_packet* packet);
