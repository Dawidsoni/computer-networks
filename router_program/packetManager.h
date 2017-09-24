//Dawid Wegner (279743)
#ifndef PACKET_MANAGER_H
#define PACKET_MANAGER_H

#include <vector>
#include <unordered_map>
#include "directConnection.h"
using namespace std;

class PacketManager {
  const static int DEFAULT_REVEICE_SECS = 15;
  unordered_map<string, DirectConnection*> directConnMap;
  unordered_map<string, Connection*> connMap;
  void fromInput();
  void updateDirectRoute(int crTime, Connection *directConn);
  void updateIndirectRoute(int crTime, Connection* srcConn, string senIp, Connection &recvConn);
  void updateRoute(int crTime, Connection* srcConn, string senIp, Connection &recvConn);
  void receiveRouteTableOnce(int crTime);
  void delConnFromList(vector<string>& eraseList);
public:
  PacketManager(bool fromInput);
  virtual ~PacketManager();
  void addDirectConn(DirectConnection* conn);
  void sendRouteTable(int curCrTime);
  void receiveRouteTable(int crTime, int receiveSecs = DEFAULT_REVEICE_SECS);
  void clearUnusedConn(int crTime);
  void printRouteTable();
};

#endif
