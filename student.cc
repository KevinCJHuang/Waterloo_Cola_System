#include "student.h"
#include "MPRNG.h"
#include "vendingMachine.h"  // access: Flavours
#include "nameServer.h"
#include "WATCardOffice.h"
#include "groupOff.h"
#include "printer.h"

extern MPRNG mprng;

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases )
: printer(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff),
id(id), maxPurchases(maxPurchases) {}


void Student::main() {
  cout << endl << "Student main starts: " << id << endl;

  unsigned int numPurchases = mprng(1, maxPurchases);
  unsigned int purchased = 0;
  VendingMachine::Flavours favFlavour = 
    static_cast<VendingMachine::Flavours>(mprng(0, 3));
  VendingMachine* vm = nameServer.getMachine(id);
  cout << endl << "Student vm got : " << id << endl;

  WATCard::FWATCard watCard = cardOffice.create(id, 5);
  cout << endl << "Student 1 cards got : " << id << endl;

  WATCard::FWATCard giftCard = groupoff.giftCard();
  cout << endl << "Student 2 cards got : " << id << endl;

  printer.print(Printer::Kind::Student, id, 'S', favFlavour, numPurchases);
  printer.print(Printer::Kind::Student, id, 'V', vm->getId());
  for ( ;; ) {
  if (purchased == numPurchases) break;
    yield(mprng(1, 10));


    bool isGiftCardPurchased = false;
    try {
      _Select (giftCard) {
        isGiftCardPurchased = true;
        vm->buy( favFlavour, *giftCard );
        printer.print(Printer::Kind::Student, id, 'G', favFlavour, (*giftCard).getBalance());
        giftCard.reset();
      }
      or _Select (watCard) {
        vm->buy( favFlavour, *watCard );
        printer.print(Printer::Kind::Student, id, 'B', favFlavour, (*watCard).getBalance());
      }
      purchased++;
    } catch (WATCardOffice::Lost&) {
      watCard = cardOffice.create(id, 5);
      printer.print(Printer::Kind::Student, id, 'L');
    } catch (VendingMachine::Funds&) {
      watCard = cardOffice.transfer(id, 5 + vm->cost(), watCard);
    } catch (VendingMachine::Stock&) {
      vm = nameServer.getMachine(id);
    } catch (VendingMachine::Free&) {
      isGiftCardPurchased
        ? printer.print(Printer::Kind::Student, id, 'a', favFlavour, (*giftCard).getBalance())
        : printer.print(Printer::Kind::Student, id, 'A', favFlavour, (*watCard).getBalance());
      yield(4);
    } // try
  }
  printer.print(Printer::Kind::Student, id, 'F');
}

