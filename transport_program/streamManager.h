//Dawid Wegner (279743)
#ifndef STREAM_MANAGER
#define STREAM_MANAGER
#include "streamReader.h"
#include "frameEntry.h"
#include <fstream>
#include <vector>
#include <map>
using namespace std;

class StreamManager {
  static const int READ_LEN = 1000;
  static const int SEND_COUNT = 10;
  static const int FRAME_SIZE = 100;
  static const int LOST_MSEC = 70;
  int framePos = 0, readProgress = 0, frameCount;
  map<int, FrameEntry> frameMap;
  StreamInfo streamInfo;
  StreamReader streamReader;
  ofstream outStream;
  void savePacket(DataPacket& packet);
  void sendFrame(FrameEntry& entry, int sendCount);
  void sendFrameList();
  void receiveFrameList();
  void clearFrameQueue();
  void updateFrameQueue();
  void logProgress();
public:
  StreamManager(StreamInfo streamInfo);
  void initStream();
  void readData();
};

#endif
