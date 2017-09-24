//Dawid Wegner (279743)
#include "streamManager.h"
#include "frameEntry.h"
#include <iostream>
#include <unistd.h>
#include <math.h>
using namespace std;

StreamManager::StreamManager(StreamInfo streamInfo) {
  this->streamInfo = streamInfo;
  this->streamReader = StreamReader(streamInfo);
}

void StreamManager::initStream() {
  streamReader.initStream();
  outStream.open(streamInfo.getFileOutput());
}

void StreamManager::savePacket(DataPacket& packet) {
  char* buff = (char*)(&packet.getDataPtr()[0]);
  outStream.write(buff, packet.getDataLen());
  outStream.flush();
}

void StreamManager::updateFrameQueue() {
  while(framePos < frameCount && frameMap.size() < FRAME_SIZE) {
    frameMap[framePos] = FrameEntry(framePos);
    framePos++;
  }
}

void StreamManager::sendFrame(FrameEntry& entry, int sendCount) {
  entry.isSent = true;
  entry.updateTime();
  int dataLen = streamInfo.getDataLength();
  int startPos = entry.framePos * READ_LEN;
  int frameLen = READ_LEN;
  if(entry.framePos + 1 == frameCount && dataLen % READ_LEN > 0) {
    frameLen = dataLen % READ_LEN;
  }
  for(int i = 0; i < sendCount; i++) {
    streamReader.sendData(startPos, frameLen);
  }
}

void StreamManager::sendFrameList() {
  int counter = 0;
  for(auto& kv : frameMap) {
    FrameEntry& entry = kv.second;
    if(entry.isSent == false || entry.getmSecFromUpdate() > LOST_MSEC) {
      float mult = 1 - ((float)counter / frameMap.size());
      sendFrame(entry, ceil(SEND_COUNT * mult));
    }
    counter++;
  }
}

void StreamManager::receiveFrameList() {
  DataPacket packet;
  while(streamReader.readData(&packet) > 0) {
    int pos = packet.getFromInd() / READ_LEN;
    if(frameMap.find(pos) != frameMap.end() && frameMap[pos].isFilled == false) {
      frameMap[pos].dataPacket = packet;
      frameMap[pos].isFilled = true;
    }
  }
}

void StreamManager::clearFrameQueue() {
  vector<int> removeList;
  for(auto& kv : frameMap) {
    if(kv.second.isFilled) {
      savePacket(kv.second.dataPacket);
      removeList.push_back(kv.first);
    }else {
      break;
    }
  }
  for(auto pos : removeList) {
    frameMap.erase(pos);
  }
}

void StreamManager::logProgress() {
  int lFrame = (frameMap.empty() ? framePos : frameMap.begin()->second.framePos);
  int curProgress = ((float)lFrame / frameCount) * 100;
  if(curProgress > readProgress) {
    readProgress = curProgress;
    cout << readProgress << "% done\n";
  }
}

void StreamManager::readData() {
  int dataLen = streamInfo.getDataLength();
  frameCount = (dataLen / READ_LEN) + (dataLen % READ_LEN > 0);
  while(framePos < frameCount || frameMap.size() > 0) {
    updateFrameQueue();
    sendFrameList();
    receiveFrameList();
    clearFrameQueue();
    logProgress();
  }
}
