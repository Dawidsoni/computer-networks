//Dawid Wegner (279743)
#ifndef ROUTER_PACKET_H
#define ROUTER_PACKET_H

#include "connection.h"

class RouterPacket {
public:
  unsigned int ip;
  unsigned char mask;
  unsigned int dist;
  RouterPacket() {}
  RouterPacket(Connection* conn);
  Connection asConnection();
};

#endif
