//Dawid Wegner (279743)
#ifndef STREAM_READER
#define STREAM_READER

#include "streamInfo.h"
#include "dataPacket.h"
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <strings.h>

class StreamReader {
  StreamInfo streamInfo;
  int sockFd;
  struct sockaddr_in serverAddr;
  void initServerAddr();
  bool isValidSender(struct sockaddr_in& sender);
public:
  StreamReader() {}
  StreamReader(StreamInfo streamInfo);
  void initStream();
  int sendData(int fromInd, int dataLen);
  int readData(DataPacket* packet);
};

#endif
