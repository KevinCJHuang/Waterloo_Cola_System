#include "vendingMachine.h";

VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ): sodaCost(sodaCost), id(id), nameServer(nameServer), printer(prt) {}


void VendingMachine::main() {
  NameServer.VMregister(this);


}


void VendingMachine::buy( Flavours flavour, WATCard & card ) {
  if (card.getBalance() < sodaCost) throw Fund();
  if (stock[flavour] <= 0) throw Stock();
  if (!mprng(4)) throw Free();
  // Kevin: Question (needs to be fixed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!)
  // From assignment: (A flag variable is necessary to know when to raise Funds, Stock or Free on the correct task stack.)
  // What does it mean?
  
  stock[flavour]--;
  card.withdraw(sodaCost);
}

unsigned int * VendingMachine::inventory();
void VendingMachine::restocked();
_Nomutex unsigned int VendingMachine::cost() const { return sodaCost; }
_Nomutex unsigned int VendingMachine::getId() const { return id; }

