//Dawid Wegner (279743)
#ifndef DIRECT_CONNECTION_H
#define DIRECT_CONNECTION_H

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <strings.h>
#include <vector>
#include "connection.h"
#include "routerPacket.h"

class DirectConnection : public Connection {
  static const int DEFAULT_PORT = 54321;
  int sockFd, port, directDist;
  struct sockaddr_in serverAddr;
protected:
  void fromInput();
  void initServerAddr();
  void initSocket();
public:
  DirectConnection(int port = DEFAULT_PORT);
  DirectConnection(bool isFromInput, int port = DEFAULT_PORT);
  virtual ~DirectConnection() {}
  int receiveRoute(char* senIp, int senIpSize, RouterPacket& packet);
  void sendRoute(unsigned char* buff, int buffSize, int curCrTime);
  virtual bool isDirectConnection();
  virtual void setDefaultDist();
  virtual void print();
};

#endif
