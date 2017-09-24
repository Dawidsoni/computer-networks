//Dawid Wegner (279743)
#ifndef FRAME_ENTRY
#define FRAME_ENTRY
#include "dataPacket.h"
#include <ctime>

class FrameEntry {
public:
  bool isSent, isFilled;
  int framePos;
  timespec startTime;
  DataPacket dataPacket;
  FrameEntry() {}
  FrameEntry(int framePos);
  void updateTime();
  int getmSecFromUpdate();
};

#endif
