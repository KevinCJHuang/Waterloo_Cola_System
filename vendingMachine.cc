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
    _Accept (~VendingMachine) {break;}
    or _Accept (buy) {
      if (curCard->getBalance() < sodaCost) {
        state = FUNDS;
      } else if (stock[curFlavour] <= 0) {
        state = STOCK;
      } else if (!mprng(4)) {
        state = FREE;
        stock[curFlavour]--;
        printer.print(Printer::Kind::Vending, id, 'A');
      } else {
        state = BUY;
        stock[curFlavour]--;
        printer.print(Printer::Kind::Vending, id, 'B', curFlavour, stock[curFlavour]);
        curCard->withdraw(sodaCost);
      }
      bench.signalBlock();
    } or _Accept (inventory) {
      printer.print(Printer::Kind::Vending, id, 'r');
      _Accept (restocked);
      printer.print(Printer::Kind::Vending, id, 'R');
    }
  } // for
  printer.print(Printer::Kind::Vending, id, 'F');
}


void VendingMachine::buy( Flavours flavour, WATCard & card ) {
  curFlavour = flavour;
  curCard = &card;
  bench.wait();
  switch (state) {
    case FUNDS:
      _Throw Funds{};
      break;
    case STOCK:
      _Throw Stock{};
      break;
    case FREE:
      _Throw Free{};
  }
}

unsigned int * VendingMachine::inventory() { return stock; }
void VendingMachine::restocked() { 
}
_Nomutex unsigned int VendingMachine::cost() const { return sodaCost; }
_Nomutex unsigned int VendingMachine::getId() const { return id; }