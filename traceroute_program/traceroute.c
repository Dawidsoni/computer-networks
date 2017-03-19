//Dawid Wegner (279743)
#include <time.h>
#include <string.h>
#include "recv_packet.h"
#include "send_packet.h"
#include "utils.h"

void retr_response(int fd, int pid, int seq, recv_packet p_arr[], int* p_count, struct timespec* t_start) {
  u_int8_t buff[IP_MAXPACKET];
  struct timespec t_end;
  do {
    if(receive_packet(fd, buff, &p_arr[*p_count]) == -1) {
      break;
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);
    if(get_pckt_pid(&p_arr[*p_count]) != pid || get_pckt_seq(&p_arr[*p_count]) < seq) {
      continue;
    }
    p_arr[*p_count].r_time = (t_end.tv_nsec - t_start->tv_nsec) / 1e6;
    (*p_count)++;
    if(*p_count >= 3) {
      break;
    }
  }while(t_end.tv_sec - t_start->tv_sec < 1);
}

int write_response(recv_packet p_arr[], int p_count) {
  if(p_count == 0) {
    printf("*\n");
    return 0;
  }
  char* s0 = p_arr[0].snd_ip, *s1 = p_arr[1].snd_ip, *s2 = p_arr[2].snd_ip;
  printf("%s ", s0);
  if(p_count > 1 && strcmp(s0, s1)) {
    printf("%s ", s1);
  }
  if(p_count > 2 && strcmp(s0, s2) && strcmp(s1, s2)) {
    printf("%s ", s2);
  }
  if(p_count < 3) {
    printf("???\n");
  }else if(p_count == 3) {
    int av_time = (p_arr[0].r_time + p_arr[1].r_time + p_arr[2].r_time) / 3;
    printf("%dms\n", av_time);
  }
  return (get_icmp_type(&p_arr[0]) == ICMP_ECHOREPLY);
}

int trace_response(int fd, int pid, int seq) {
  struct timespec t_start, t_end;
  recv_packet p_arr[3];
  int p_count = 0;
  clock_gettime(CLOCK_MONOTONIC_RAW, &t_start);
  clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);
  do {
    int usec_interval = (t_end.tv_nsec - t_start.tv_nsec) / 1e3;
    recv_config recv_cfg = get_recv_config(fd, 1e6 - usec_interval);
    int r_count = wait_for_packets(&recv_cfg);
    if(r_count == 0) {
      break;
    }
    retr_response(fd, pid, seq, p_arr, &p_count, &t_start);
    if(p_count == 3) {
      break;
    }
    clock_gettime(CLOCK_MONOTONIC_RAW, &t_end);
  }while(t_end.tv_sec - t_start.tv_sec < 1);
  return write_response(p_arr, p_count);
}

int trace_for_ttl(int fd, int pid, int seq, char* ip_addr, int ttl) {
  for(int i = 0; i <= 2; i++) {
    struct icmphdr icmp_header = create_icmp_header(pid, seq + i);
    send_packet(fd, ip_addr, &icmp_header, ttl);
  }
  return trace_response(fd, pid, seq);
}

void traceroute(char* ip_addr) {
  int sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  int pid = getpid();
  int seq = 0;
  for(int ttl = 1; ttl <= 30; ttl++) {
    printf("%d. ", ttl);
    if(trace_for_ttl(sock_fd, pid, seq, ip_addr, ttl)) {
	    break;
    }
    seq += 3;
  }
}

int main(int argc, char* argv[]) {
  if(argc < 2) {
    printf("Error: too few input arguments\n");
    exit(EXIT_FAILURE);
  }
  char* ip_addr = argv[1];
  if(is_ip_addr(ip_addr) == 0) {
    printf("Error: provided input argument is not IP address\n");
    exit(EXIT_FAILURE);
  }
  traceroute(ip_addr);
  exit(EXIT_SUCCESS);
}
