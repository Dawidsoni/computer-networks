//Dawid Wegner (279743)
#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
using namespace std;

class Connection {
protected:
  const static int INFITITY_DIST = 32;
  const static int EXPIRE_TIME_LIMIT = 3;
  void fromInput();
public:
  string ipAddr, ipMask, viaIp;
  int dist, crTime;
  Connection();
  Connection(bool isFromInput);
  Connection(string ipAddr, string ipMask, string viaIp, int dist, int crTime);
  virtual ~Connection() {}
  string getBroadcastAddr();
  string getWebAddr();
  unsigned int maskAsSubnet();
  unsigned int ipAsNumber();
  void setInfinityDist();
  bool isInfinityDist();
  bool isExpired(int crTime);
  virtual bool isDirectConnection();
  virtual void setDefaultDist();
  virtual void print();
};

#endif
