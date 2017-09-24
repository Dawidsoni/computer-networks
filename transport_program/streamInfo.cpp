//Dawid Wegner (279743)
#include "streamInfo.h"
#include <stdexcept>
using namespace std;

StreamInfo::StreamInfo(string serverAddr, int port, string outPath, int dataLen) {
  this->serverAddr = serverAddr;
  if(isValidPort(port) == false) {
    throw invalid_argument("Port number is invalid");
  }
  if(dataLen < 0) {
    throw invalid_argument("Data length is invalid");
  }
  this->port = port;
  this->outPath = outPath;
  this->dataLen = dataLen;
}

string StreamInfo::getSeverAddr() {
  return serverAddr;
}

int StreamInfo::getPort() {
  return port;
}

string StreamInfo::getFileOutput() {
  return outPath;
}

int StreamInfo::getDataLength() {
  return dataLen;
}

bool StreamInfo::isValidPort(int port) {
  return (port >= 0 && port < (1 << 16));
}
