//Dawid Wegner (279743)
#include "recv_packet.h"

recv_config get_recv_config(int sock_fd, int usec_limit) {
  recv_config config;
  config.sock_fd = sock_fd;
  FD_ZERO(&config.descs);
  FD_SET(sock_fd, &config.descs);
  config.t_limit.tv_sec = 0;
  config.t_limit.tv_usec = usec_limit;
  return config;
}

int wait_for_packets(recv_config* cfg) {
  int ready = select(cfg->sock_fd + 1, &cfg->descs, NULL, NULL, &cfg->t_limit);
  if(ready < 0) {
    printf("Error during 'select' call\n");
    exit(EXIT_FAILURE);
  }
  return ready;
}

int receive_packet(int fd, u_int8_t buff[], recv_packet* packet) {
  struct sockaddr_in sender;
  struct sockaddr* s_addr = (struct sockaddr*)&sender;
  socklen_t s_len = sizeof(sender);
  ssize_t p_len = recvfrom(fd, buff, IP_MAXPACKET, MSG_DONTWAIT, s_addr, &s_len);
  if(p_len < 0) {
    return -1;
  }
  inet_ntop(AF_INET, &(sender.sin_addr), packet->snd_ip, sizeof(packet->snd_ip));
  packet->ip_header = (struct iphdr*)buff;
  buff += 4 * packet->ip_header->ihl;
  if(((struct icmphdr*)buff)->type == ICMP_ECHOREPLY) {
    packet->packet_data = (struct icmp*)buff;
  }else {
    struct ip* ip_data = (struct ip*)(buff + 8);
    packet->packet_data = (struct icmp*)(buff + 8 + ip_data->ip_hl * 4);
  }
  return 0;
}

int get_pckt_pid(recv_packet* packet) {
  return packet->packet_data->icmp_id;
}

int get_pckt_seq(recv_packet* packet) {
  return packet->packet_data->icmp_seq;
}

int get_icmp_type(recv_packet* packet) {
  return packet->packet_data->icmp_type;
}
