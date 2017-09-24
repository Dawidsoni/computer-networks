//Dawid Wegner (279743)
#include "frameEntry.h"

FrameEntry::FrameEntry(int framePos) {
  this->isSent = false;
  this->isFilled = false;
  this->framePos = framePos;
  updateTime();
}

void FrameEntry::updateTime() {
  clock_gettime(CLOCK_REALTIME, &startTime);
}

int FrameEntry::getmSecFromUpdate() {
  timespec curTime;
  clock_gettime(CLOCK_REALTIME, &curTime);
  int result = (curTime.tv_sec - startTime.tv_sec) * 1000;
  result += (curTime.tv_nsec - startTime.tv_nsec) / 1000000;
  return result;
}
