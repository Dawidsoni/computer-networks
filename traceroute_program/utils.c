//Dawid Wegner (279743)
#include "utils.h"

u_int16_t compute_icmp_checksum (const void* buff, int length) {
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert(length % 2 == 0);
	for (sum = 0; length > 0; length -= 2) {
		sum += *ptr++;
  }
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

int is_ip_addr(char ip_addr[]) {
  struct sockaddr_in sock_addr;
  int result = inet_pton(AF_INET, ip_addr, &(sock_addr.sin_addr));
  return (result != 0);
}
