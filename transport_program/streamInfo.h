//Dawid Wegner (279743)
#ifndef STREAM_INFO
#define STREAM_INFO

#include <string>
using namespace std;

class StreamInfo {
  string serverAddr;
  int port;
  string outPath;
  int dataLen;
public:
  StreamInfo() {}
  StreamInfo(string serverAddr, int port, string outPath, int dataLen);
  string getSeverAddr();
  int getPort();
  string getFileOutput();
  int getDataLength();
  bool isValidPort(int port);
};

#endif
