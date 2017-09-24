//Dawid Wegner (279743)
#include "dataPacket.h"
#include <cstddef>
#include <iostream>
using namespace std;

DataPacket::DataPacket(char* buff, int buffSize) {
  initFromBuff(buff, buffSize);
}

int DataPacket::readIntFromBuff(char* buff, int buffSize, int& pos) {
  string result;
  while(pos < buffSize) {
    if(buff[pos] == ' ' || buff[pos] == '\n') {
      break;
    }else {
      result += buff[pos];
    }
    pos++;
  }
  pos++;
  return atoi(result.c_str());
}

void DataPacket::initFromBuff(char* buff, int buffSize) {
  data.clear();
  int pos = 5;
  fromInd = readIntFromBuff(buff, buffSize, pos);
  dataLen = readIntFromBuff(buff, buffSize, pos);
  for(int i = 0; i < dataLen; i++) {
    data.push_back(buff[pos + i]);
  }
}

int DataPacket::getFromInd() {
  return fromInd;
}

int DataPacket::getDataLen() {
  return dataLen;
}

vector<char>& DataPacket::getDataPtr() {
  return data;
}
