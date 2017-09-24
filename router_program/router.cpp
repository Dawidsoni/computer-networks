//Dawid Wegner (279743)
#include "packetManager.h"
#include <iostream>

void runRouter() {
  PacketManager manager(true);
  int crTime = 1;
  while(true) {
    manager.sendRouteTable(crTime);
    manager.receiveRouteTable(crTime);
    manager.printRouteTable();
    manager.clearUnusedConn(crTime);
    crTime++;
  }
}

int main() {
  try {
    runRouter();
  }catch(exception& exc) {
    cout << exc.what() << "\n";
  }
}
