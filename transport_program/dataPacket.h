//Dawid Wegner (279743)
#ifndef DATA_PACKET
#define DATA_PACKET
#include <netinet/ip.h>
#include <vector>
using namespace std;

class DataPacket {
  int fromInd;
  int dataLen;
  vector<char> data;
  int readIntFromBuff(char* buff, int buffSize, int& pos);
public:
  DataPacket() {}
  DataPacket(char* buff, int buffSize);
  void initFromBuff(char* buff, int buffSize);
  int getFromInd();
  int getDataLen();
  vector<char>& getDataPtr();
};

#endif
