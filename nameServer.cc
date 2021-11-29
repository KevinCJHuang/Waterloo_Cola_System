#include "nameServer.h"
#include "vendingMachine.h"
#include "printer.h"
#include "WATCard.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents )
  :printer(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
    // machineList = new VendingMachine* [numVendingMachines];
    studVMs = new unsigned int [numStudents];
}

NameServer::~NameServer() {
  delete [] machineList;
  delete [] studVMs;
}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
  // curId = vendingmachine->getId();
  machineList[registeredMachines++] = vendingmachine;
  printer.print(Printer::Kind::NameServer, 'R', vendingmachine->getId());
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

  // unsigned int tempStudVMs [numStudents]; // fixed
  // studVMs = tempStudVMs; // points to local array

  // new unsigned int [numStudents];
  for (unsigned i = 0; i < numStudents; i++) {
    studVMs[i] = i % numVendingMachines;
  }

  for (unsigned i = 0; i < numVendingMachines; i++) {
    _Accept (VMregister);
    // printer.print(Printer::Kind::NameServer, 'R', curId);
  }

  for ( ;; ) {
    _Accept (~NameServer) { break; }
    or _Accept (getMachineList) {}
    or _Accept (getMachine) {
      printer.print(Printer::Kind::NameServer, 'N', curId, machineList[studVMs[curId]]->getId());
      studVMs[curId] = (studVMs[curId] + 1) % numVendingMachines;
    }
  }
  printer.print(Printer::Kind::NameServer, 'F');
}
