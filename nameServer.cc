#include "nameServer.h"
#include "vendingMachine.h"
#include "printer.h"
#include "WATCard.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents )
  :printer(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
}

NameServer::~NameServer() {
  delete [] machineList;
}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
  curId = vendingmachine->getId();
  machineList[curId] = vendingmachine;
  printer.print(Printer::Kind::NameServer, 'R', curId);
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
  curId = id;
  return machineList[studVMs[id]];
}

VendingMachine ** NameServer::getMachineList() {
  return machineList;
}

void NameServer::main() {
  printer.print(Printer::Kind::NameServer, 'S');
  // VendingMachine* tempMachineList [numVendingMachines];
  machineList = new VendingMachine* [numVendingMachines];

  unsigned int tempStudVMs [numStudents]; // fixed
  studVMs = tempStudVMs; // points to local array

  // new unsigned int [numStudents];
  for (unsigned i = 0; i < numStudents; i++) {
    studVMs[i] = i % numVendingMachines;
  }

  for (unsigned i = 0; i < numVendingMachines; i++) {
    _Accept (VMregister);
  }

  for ( ;; ) {
    _Accept (~NameServer) { break; }
    or _Accept (getMachineList) {}
    or _Accept (getMachine) {
      printer.print(Printer::Kind::NameServer, 'N', curId, studVMs[curId]);
      studVMs[curId] = (studVMs[curId] + 1) % numVendingMachines;
    }
  }
  printer.print(Printer::Kind::NameServer, 'F');
}
