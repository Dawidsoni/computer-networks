//Dawid Wegner (279743)
#include <iostream>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include "connection.h"

void Connection::fromInput() {
  string ipWithMask, distKeyword;
  cin >> ipWithMask >> distKeyword >> dist;
  ipAddr = ipWithMask.substr(0, ipWithMask.find("/"));
  ipMask = ipWithMask.substr(ipWithMask.find("/") + 1);
  viaIp = "";
}

string Connection::getBroadcastAddr() {
  unsigned int ipNumber = ipAsNumber();
  unsigned int subnet = maskAsSubnet();
  unsigned int broadcast = ipNumber | (~htonl(subnet));
  char result[20];
  inet_ntop(AF_INET, &(broadcast), result, INET_ADDRSTRLEN);
  return string(result);
}

string Connection::getWebAddr() {
  unsigned int ipNumber = ipAsNumber();
  unsigned int subnet = maskAsSubnet();
  unsigned int webAddr = ipNumber & htonl(subnet);
  char result[20];
  inet_ntop(AF_INET, &(webAddr), result, INET_ADDRSTRLEN);
  return string(result);
}

Connection::Connection() {
  this->ipAddr = "";
  this->ipMask = "";
  this->viaIp = "";
  this->dist = 0;
  this->crTime = 0;
}

Connection::Connection(bool isFromInput) {
  if(isFromInput) {
    fromInput();
  }else {
    Connection();
  }
}

Connection::Connection(string ipAddr, string ipMask, string viaIp, int dist, int crTime) {
  this->ipAddr = ipAddr;
  this->ipMask = ipMask;
  this->viaIp = viaIp;
  this->dist = dist;
  this->crTime = crTime;
}

unsigned int Connection::maskAsSubnet() {
  unsigned int result = stoi(ipMask);
  if(result != 0) {
    result = (0xFFFFFFFF << (32 - result)) & 0xFFFFFFFF;
  }
  return result;
}

unsigned int Connection::ipAsNumber() {
  struct sockaddr_in addr;
  inet_pton(AF_INET, ipAddr.c_str(), &(addr.sin_addr));
  return addr.sin_addr.s_addr;
}

void Connection::setInfinityDist() {
  dist = INFITITY_DIST;
}

bool Connection::isInfinityDist() {
  return (dist >= INFITITY_DIST);
}

bool Connection::isExpired(int crTime) {
  return (crTime - this->crTime >= EXPIRE_TIME_LIMIT);
}

bool Connection::isDirectConnection() {
  return false;
}

void Connection::setDefaultDist() {
  dist = 0;
}

void Connection::print() {
  if(dist < INFITITY_DIST) {
    cout << getWebAddr() << "/" << ipMask << " ";
    cout << "distance " << dist << " via " << viaIp << "\n";
  }
}
