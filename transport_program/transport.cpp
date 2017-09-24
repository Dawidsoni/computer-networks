//Dawid Wegner (279743)
#include "streamInfo.h"
#include "streamManager.h"
#include <iostream>
#include <stdlib.h>
#include <stdexcept>
using namespace std;

StreamInfo getStreamInfo(int argc, char* argv[]) {
  if(argc < 4) {
    throw invalid_argument("Too few program parameters");
  }
  int port = atoi(argv[1]);
	string outPath = argv[2];
	int dataLen = atoi(argv[3]);
  return StreamInfo("156.17.4.30", port, outPath, dataLen);
}

int main(int argc, char* argv[]) {
  try {
    auto streamInfo = getStreamInfo(argc, argv);
    StreamManager streamManager(streamInfo);
    streamManager.initStream();
    streamManager.readData();
  }catch(exception& exc) {
    cout << "Error: " << exc.what() << "\n";
  }
  return 0;
}
