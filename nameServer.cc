#include "nameServer.h"
#include "vendingMachine.h"
#include "printer.h"
#include "WATCard.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents )
  :printer(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
    machineList = new VendingMachine*[numVendingMachines];
    studVMs = new unsigned int [numStudents];
    for (unsigned i = 0; i < numStudents; i++) {
      studVMs[i] = i % numVendingMachines;
    }
}

NameServer::~NameServer() {
  delete machineList;
  delete studVMs;
}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
  curId = vendingmachine->getId();
  machineList[curId] = vendingmachine;
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
  for (unsigned i = 0; i < numVendingMachines; i++) {
    _Accept (VMregister);
    printer.print(Printer::Kind::NameServer, 'R', curId);
  }

  for ( ;; ) {
    _Accept (getMachineList) {}
    or _Accept (getMachine) {
      printer.print(Printer::Kind::NameServer, 'N', curId, studVMs[curId]);
      studVMs[curId] = (studVMs[curId] + 1) % numVendingMachines;
    }
  }
  printer.print(Printer::Kind::NameServer, 'F');
}
