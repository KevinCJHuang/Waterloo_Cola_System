#include "nameServer.h"
#include "vendingMachine.h"
#include "printer.h"
#include "WATCard.h"

NameServer::NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents )
  :printer(prt), numVendingMachines(numVendingMachines), numStudents(numStudents) {
    studVMs = new unsigned int [numStudents];
    machineList = new VendingMachine* [numVendingMachines];
}

NameServer::~NameServer() {
  delete [] machineList;
  delete [] studVMs;
}

void NameServer::VMregister( VendingMachine * vendingmachine ) {
  curId = vendingmachine->getId();  // get vm's id for print msg
  machineList[registeredMachines++] = vendingmachine;
}

VendingMachine * NameServer::getMachine( unsigned int id ) {
  curId = id;                       // get student's id for print msg
  return machineList[studVMs[id]];  // return its assigned vm
}

VendingMachine ** NameServer::getMachineList() {
  return machineList;
}

void NameServer::main() {
  printer.print(Printer::Kind::NameServer, 'S');

  // assign an index of the machineList to each student
  for (unsigned i = 0; i < numStudents; i++) {
    studVMs[i] = i % numVendingMachines;
  }

  // register vm
  for (unsigned i = 0; i < numVendingMachines; i++) {
    _Accept (VMregister);
    printer.print(Printer::Kind::NameServer, 'R', curId);
  }

  // Accept calls & return vm positions
  for ( ;; ) {
    _Accept (~NameServer) { break; } // break when dtor is called
    or _Accept (getMachineList) {}   // returns vm list
    or _Accept (getMachine) {
      printer.print(Printer::Kind::NameServer, 'N', curId, machineList[studVMs[curId]]->getId());
      // update the vm assigned to the student
      studVMs[curId] = (studVMs[curId] + 1) % numVendingMachines;
    } // _Accept
  } // for
  printer.print(Printer::Kind::NameServer, 'F');
}
