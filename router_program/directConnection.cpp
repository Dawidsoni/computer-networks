//Dawid Wegner (279743)
#include "directConnection.h"
#include <iostream>
#include <exception>
using namespace std;

void DirectConnection::fromInput() {
  Connection::fromInput();
  directDist = dist;
  initSocket();
}

void DirectConnection::initServerAddr() {
  bzero(&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);
}

void DirectConnection::initSocket() {
  sockFd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockFd < 0) {
    throw runtime_error("Error while creating socket");
  }
  initServerAddr();
  string broadcastIp = getBroadcastAddr();
  inet_pton(AF_INET, broadcastIp.c_str(), &serverAddr.sin_addr);
  int isBr = 1;
  setsockopt(sockFd, SOL_SOCKET, SO_BROADCAST, (void*)&isBr, sizeof(isBr));
  if(bind(sockFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
    throw runtime_error("Error while binding");
  }
}

DirectConnection::DirectConnection(int port) {
  this->port = port;
  this->directDist = 0;
}

DirectConnection::DirectConnection(bool isFromInput, int port) {
  this->port = port;
  if(isFromInput) {
    fromInput();
  }else {
    Connection(false);
    this->directDist = 0;
  }
}

int DirectConnection::receiveRoute(char* senIp, int senIpSize, RouterPacket& packet) {
  unsigned char buff[IP_MAXPACKET + 1];
  struct sockaddr_in sender;
  struct sockaddr* senAddr = (struct sockaddr*)(&sender);
  socklen_t senSize = sizeof(sender);
  int rSize = recvfrom(sockFd, buff, IP_MAXPACKET, MSG_DONTWAIT, senAddr, &senSize);
  if(rSize < 0) {
    return rSize;
  }
  inet_ntop(AF_INET, &(sender.sin_addr), senIp, senIpSize);
  packet = *((RouterPacket*)buff);
  return rSize;
}

void DirectConnection::sendRoute(unsigned char* buff, int buffSize, int curCrTime) {
  struct sockaddr* sockAddr = (struct sockaddr*)(&serverAddr);
  int sentSize = sendto(sockFd, buff, buffSize, 0, sockAddr, sizeof(serverAddr));
  if(sentSize != buffSize && isInfinityDist() == false) {
    dist = INFITITY_DIST;
    crTime = curCrTime;
  }
}

bool DirectConnection::isDirectConnection() {
  return true;
}

void DirectConnection::setDefaultDist() {
  dist = directDist;
}

void DirectConnection::print() {
  cout << getWebAddr() << "/" << ipMask << " ";
  if(dist == directDist) {
    cout << "distance " << dist << " ";
  }else {
    cout << "unreachable ";
  }
  cout << "connected directly\n";
}
