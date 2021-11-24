#include "vendingMachine.h"
#include "MPRNG.h"
#include "WATCard.h"
#include "nameServer.h"
extern MPRNG mprng;

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost ): sodaCost(sodaCost), id(id), nameServer(nameServer), printer(prt) {}


void VendingMachine::main() {
  nameServer.VMregister(this);
  printer.print(Printer::Kind::Vending, 'S', sodaCost);
  for ( ;; ) {
    try {
      _Accept (buy) {
        stock[lastFlavour]--;
      } or _Accept (inventory) {
        _Accept (restocked);
      }
    } catch (uMutexFailure::RendezvousFailure &) {
      if (isFree) { // Free drink; needs to decrement stock
        stock[lastFlavour]--;
        isFree = false;
      }
    } // try
  } // for
  printer.print(Printer::Kind::Vending, 'F');
}


void VendingMachine::buy( Flavours flavour, WATCard & card ) {
  lastFlavour = flavour;
  if (card.getBalance() < sodaCost) throw Funds();
  if (stock[flavour] <= 0) throw Stock();
  if (!mprng(4)) {
    isFree = true;
    printer.print(Printer::Kind::Vending, 'A');
    throw Free();
  } else {
    printer.print(Printer::Kind::Vending, 'B', flavour, stock[flavour]);
  }
  card.withdraw(sodaCost);
}

unsigned int * VendingMachine::inventory() { return stock; }
void VendingMachine::restocked() { 
  printer.print(Printer::Kind::Vending, 'r');
  printer.print(Printer::Kind::Vending, 'R');
}
_Nomutex unsigned int VendingMachine::cost() const { return sodaCost; }
_Nomutex unsigned int VendingMachine::getId() const { return id; }

