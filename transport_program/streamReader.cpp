//Dawid Wegner (279743)
#include "streamReader.h"
#include <stdexcept>
#include <iostream>
#include <string.h>
using namespace std;

StreamReader::StreamReader(StreamInfo streamInfo) {
  this->streamInfo = streamInfo;
}

void StreamReader::initServerAddr() {
  bzero(&serverAddr, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(streamInfo.getPort());
}


void StreamReader::initStream() {
  sockFd = socket(AF_INET, SOCK_DGRAM, 0);
  if(sockFd < 0) {
    throw runtime_error("Error while creating socket");
  }
  initServerAddr();
  string ipAddr = streamInfo.getSeverAddr();
  inet_pton(AF_INET, ipAddr.c_str(), &serverAddr.sin_addr);
  auto sockAddr = (struct sockaddr*)&serverAddr;
  int isC = connect(sockFd, sockAddr, sizeof(serverAddr));
  if(isC < 0) {
    throw runtime_error("Error while connecting to server");
  }
}

int StreamReader::sendData(int fromInd, int dataLen) {
  char buff[32];
  int buffSize = sprintf(buff, "GET %d %d\n", fromInd, dataLen);
  auto sockAddr = (struct sockaddr*)&serverAddr;
	return sendto(sockFd, buff, buffSize, 0, sockAddr, sizeof(serverAddr));
}

bool StreamReader::isValidSender(struct sockaddr_in& sender) {
  char senIp[20];
  inet_ntop(AF_INET, &(sender.sin_addr), senIp, sizeof(senIp));
  if(strcmp(senIp, streamInfo.getSeverAddr().c_str()) != 0) {
    return false;
  }
  if(htons(sender.sin_port) != streamInfo.getPort()) {
    return false;
  }
  return true;
}

int StreamReader::readData(DataPacket* packet) {
  char buff[IP_MAXPACKET];
  struct sockaddr_in sender;
  struct sockaddr* senAddr = (struct sockaddr*)(&sender);
  socklen_t senSize = sizeof(sender);
  int rSize = recvfrom(sockFd, buff, IP_MAXPACKET, MSG_DONTWAIT, senAddr, &senSize);
  if(rSize < 0 || isValidSender(sender) == false) {
    return -1;
  }
  packet->initFromBuff(buff, rSize);
  return rSize;
}
