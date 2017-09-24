//Dawid Wegner (279743)
#include "routerPacket.h"
#include <string.h>
#include <limits.h>
#include <arpa/inet.h>

RouterPacket::RouterPacket(Connection* conn) {
  ip = conn->ipAsNumber();
  mask = stoi(conn->ipMask);
  if(conn->isInfinityDist()) {
    dist = UINT_MAX;
  }else {
    dist = htonl(conn->dist);
  }
}

Connection RouterPacket::asConnection() {
  Connection result;
  char ipAddr[20];
  inet_ntop(AF_INET, &(ip), ipAddr, INET_ADDRSTRLEN);
  result.ipAddr = ipAddr;
  result.ipMask = to_string(mask);
  if(dist == UINT_MAX) {
    result.setInfinityDist();
  }else {
    result.dist = htonl(dist);
  }
  return result;
}
