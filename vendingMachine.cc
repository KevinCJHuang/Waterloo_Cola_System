#include "vendingMachine.h"
#include "MPRNG.h"
#include "WATCard.h"
#include "nameServer.h"
#include "printer.h"
extern MPRNG mprng;
#ifdef DEBUG
    using namespace std;
#endif

VendingMachine::VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, 
unsigned int sodaCost ): sodaCost(sodaCost), id(id), nameServer(nameServer), printer(prt) {}

void VendingMachine::main() {
  nameServer.VMregister(this);
  printer.print(Printer::Kind::Vending, id, 'S', sodaCost);

  for ( ;; ) {
    try {
      _Accept (~VendingMachine) {break;}
      or _Accept (buy) {
        stock[lastFlavour]--;
      } or _Accept (inventory) {
        printer.print(Printer::Kind::Vending, id, 'r');
        _Accept (restocked);
        printer.print(Printer::Kind::Vending, id, 'R');
      }
    } catch (uMutexFailure::RendezvousFailure &) {
      if (isFree) { // Free drink; needs to decrement stock
        stock[lastFlavour]--;
        isFree = false;
      }
    } // try
  } // for
  printer.print(Printer::Kind::Vending, id, 'F');
}


void VendingMachine::buy( Flavours flavour, WATCard & card ) {
  lastFlavour = flavour;
  if (card.getBalance() < sodaCost) throw Funds();
  if (stock[flavour] <= 0) throw Stock();
  if (!mprng(4)) {
    isFree = true;
    printer.print(Printer::Kind::Vending, id, 'A');
    throw Free();
  } else {
    printer.print(Printer::Kind::Vending, id, 'B', flavour, stock[flavour]);
  }
  card.withdraw(sodaCost);
}

unsigned int * VendingMachine::inventory() { return stock; }
void VendingMachine::restocked() { 
}
_Nomutex unsigned int VendingMachine::cost() const { return sodaCost; }
_Nomutex unsigned int VendingMachine::getId() const { return id; }

