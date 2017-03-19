//Dawid Wegner (279743)
#include "send_packet.h"
#include "utils.h"

struct icmphdr create_icmp_header(int pid, int seq) {
  struct icmphdr result;
  result.type = ICMP_ECHO;
  result.code = 0;
  result.un.echo.id = pid;
  result.un.echo.sequence = seq;
  result.checksum = 0;
  result.checksum = compute_icmp_checksum((u_int16_t*)&result, sizeof(result));
  return result;
}

void send_packet(int fd, char ip_addr[], struct icmphdr* icmp_header, int ttl) {
  struct sockaddr_in receiver;
  size_t icmp_len = sizeof(*icmp_header);
  bzero(&receiver, sizeof(receiver));
  receiver.sin_family = AF_INET;
  inet_pton(AF_INET, ip_addr, &(receiver.sin_addr));
  setsockopt(fd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
  struct sockaddr* s_addr = (struct sockaddr*)&receiver;
  ssize_t r_len = sendto(fd, icmp_header, icmp_len, 0, s_addr, sizeof(receiver));
  if(r_len < 0) {
    printf("Error during 'sendto' call\n");
    exit(EXIT_FAILURE);
  }
}
