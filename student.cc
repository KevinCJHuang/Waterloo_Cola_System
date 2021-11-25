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
  unsigned int numPurchases = mprng(1, maxPurchases);
  VendingMachine::Flavours favFlavour = 
    static_cast<VendingMachine::Flavours>(mprng(0, 3));
  printer.print(Printer::Kind::Student, id, 'S', favFlavour, numPurchases);

  VendingMachine* vm = nameServer.getMachine(id);
  printer.print(Printer::Kind::Student, id, 'V', vm->getId());

  WATCard::FWATCard watCard = cardOffice.create(id, 5);
  WATCard::FWATCard giftCard = groupoff.giftCard();

  unsigned int purchased = 0;
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
      watCard = cardOffice.transfer(id, 5 + vm->cost(), watCard());
    } catch (VendingMachine::Stock&) {
      vm = nameServer.getMachine(id);
      printer.print(Printer::Kind::Student, id, 'V', vm->getId());
    } catch (VendingMachine::Free&) {
      isGiftCardPurchased
        ? printer.print(Printer::Kind::Student, id, 'a', favFlavour, (*giftCard).getBalance())
        : printer.print(Printer::Kind::Student, id, 'A', favFlavour, (*watCard).getBalance());
      yield(4);
    } // try
  }
  printer.print(Printer::Kind::Student, id, 'F');
}

