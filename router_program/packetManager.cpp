//Dawid Wegner (279743)
#include "packetManager.h"
#include "routerPacket.h"
#include <iostream>
#include <unordered_set>
#include <ctime>
using namespace std;

void PacketManager::fromInput() {
  int inputSize;
  cin >> inputSize;
  for(int i = 0; i < inputSize; i++) {
    DirectConnection *conn = new DirectConnection(true);
    addDirectConn(conn);
  }
}

void PacketManager::receiveRouteTableOnce(int crTime) {
  for(auto& kvConn : directConnMap) {
    char senIp[20];
    RouterPacket packet;
    while(true) {
      if(kvConn.second->receiveRoute(senIp, sizeof(senIp), packet) < 0) {
        break;
      }
      Connection recvConn = packet.asConnection();
      updateRoute(crTime, kvConn.second, senIp, recvConn);
    }
  }
}

PacketManager::PacketManager(bool isFromInput) {
  if(isFromInput) {
    fromInput();
  }
}

PacketManager::~PacketManager() {
  unordered_set<Connection*> erasedSet;
  for(auto& kvConn : directConnMap) {
    erasedSet.insert(kvConn.second);
    delete kvConn.second;
  }
  for(auto& kvConn : connMap) {
    if(erasedSet.find(kvConn.second) == erasedSet.end()) {
      delete kvConn.second;
    }
  }
}

void PacketManager::addDirectConn(DirectConnection* conn) {
  directConnMap[conn->ipAddr] = conn;
  connMap[conn->getWebAddr()] = conn;
}

void PacketManager::sendRouteTable(int curCrTime) {
  for(auto& kvConn : directConnMap) {
    for(auto& sendConn : connMap){
      RouterPacket packet(sendConn.second);
      unsigned char* buff = (unsigned char*)(&packet);
      kvConn.second->sendRoute(buff, sizeof(packet), curCrTime);
    }
  }
}

void PacketManager::receiveRouteTable(int crTime, int receiveSecs) {
  time_t startTime = time(NULL);
  while(time(NULL) - startTime < receiveSecs) {
    receiveRouteTableOnce(crTime);
  }
}

void PacketManager::printRouteTable() {
  if(connMap.empty()) {
    cout << "Empty list\n";
    return;
  }
  for(auto& kvConn : connMap) {
    kvConn.second->print();
  }
  cout << "\n";
}

void PacketManager::updateDirectRoute(int crTime, Connection *directConn) {
  string webAddr = directConn->getWebAddr();
  if(connMap.find(webAddr) != connMap.end()) {
    Connection *destConn = connMap[webAddr];
    if(destConn->crTime == crTime && destConn->dist < directConn->dist) {
      return;
    }else if(connMap[webAddr]->isDirectConnection() == false) {
      delete connMap[webAddr];
    }
  }
  connMap[webAddr] = directConn;
  connMap[webAddr]->setDefaultDist();
  connMap[webAddr]->crTime = crTime;
}

void PacketManager::updateIndirectRoute(int crTime, Connection* srcConn, string senIp, Connection &recvConn) {
  string webAddr = recvConn.getWebAddr();
  if(connMap.find(webAddr) == connMap.end()) {
    connMap[webAddr] = new Connection();
  }else {
    Connection *destConn = connMap[webAddr];
    if(destConn->crTime == crTime && destConn->dist < recvConn.dist + srcConn->dist) {
      return;
    }else if(connMap[webAddr]->isDirectConnection() == false) {
      delete connMap[webAddr];
    }
    connMap[webAddr] = new Connection();
  }
  *connMap[webAddr] = recvConn;
  connMap[webAddr]->dist += srcConn->dist;
  connMap[webAddr]->viaIp = senIp;
  if(connMap[webAddr]->isInfinityDist() == false) {
    connMap[webAddr]->crTime = crTime;
  }
}

void PacketManager::updateRoute(int crTime, Connection* srcConn, string senIp, Connection &recvConn) {
  if(directConnMap.find(senIp) != directConnMap.end()) {
    return;
  }
  string webAddr = recvConn.getWebAddr();
  if(recvConn.isInfinityDist() && connMap.find(webAddr) == connMap.end()) {
    return;
  }
  if(srcConn->getWebAddr() == recvConn.getWebAddr()) {
    updateDirectRoute(crTime, srcConn);
  }else {
    updateIndirectRoute(crTime, srcConn, senIp, recvConn);
  }
}

void PacketManager::delConnFromList(vector<string>& eraseList) {
  for(int i = 0; i < (int)eraseList.size(); i++) {
    if(connMap[eraseList[i]]->isDirectConnection() == false) {
      delete connMap[eraseList[i]];
    }
    connMap.erase(eraseList[i]);
  }
}

void PacketManager::clearUnusedConn(int crTime) {
  vector<string> eraseList;
  for(auto& kvConn : connMap) {
    Connection *conn = kvConn.second;
    if(conn->isExpired(crTime) == false) {
      continue;
    }
    if(conn->isDirectConnection() && conn->isInfinityDist() == false) {
      conn->setInfinityDist();
      conn->crTime = crTime;
    }else {
      eraseList.push_back(kvConn.first);
    }
  }
  delConnFromList(eraseList);
}
