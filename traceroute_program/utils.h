//Dawid Wegner (279743)
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <assert.h>

u_int16_t compute_icmp_checksum (const void* buff, int length);
int is_ip_addr(char ip_addr[]);
