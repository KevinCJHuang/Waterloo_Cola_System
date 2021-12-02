#include "student.h"
#include "MPRNG.h"
#include "nameServer.h"
#include "WATCardOffice.h"
#include "groupOff.h"
#include "printer.h"
#include "vendingMachine.h"

extern MPRNG mprng;

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases )
: printer(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff),
id(id) {
  numPurchases = mprng(1, maxPurchases);
}


void Student::main() {
  VendingMachine::Flavours favFlavour
    = static_cast<VendingMachine::Flavours>(mprng(0, 3)); // Selecting favourite flavour
  printer.print(Printer::Kind::Student, id, 'S', favFlavour, numPurchases);
  WATCard::FWATCard watCard = cardOffice.create(id, 5);   // Create Watcard
  WATCard::FWATCard giftCard = groupoff.giftCard();       // Create gift card

  VendingMachine* vm = nameServer.getMachine(id);         // Obtain location of vm
  printer.print(Printer::Kind::Student, id, 'V', vm->getId());

  unsigned int purchased = 0;   // Purchased drink so far
  for ( ; purchased < numPurchases; ) {
    yield(mprng(1, 10));
    bool isGiftCardPurchased = false;
    try {
      _Select (giftCard) {      // Wait for gift card
        isGiftCardPurchased = true;
        vm->buy( favFlavour, *giftCard );
        printer.print(Printer::Kind::Student, id, 'G', favFlavour, (*giftCard).getBalance());
        delete giftCard;
        giftCard.reset();       // Discard gift card
      }
      or _Select (watCard) {    // Wait for WATCard
        vm->buy( favFlavour, *watCard );
        printer.print(Printer::Kind::Student, id, 'B', favFlavour, (*watCard).getBalance());
      }
      purchased++;
    } catch (WATCardOffice::Lost&) {    // WATCard is lost
      printer.print(Printer::Kind::Student, id, 'L');
      watCard = cardOffice.create(id, 5);
    } catch (VendingMachine::Funds&) {  // WATCard balance is insufficient
      watCard = cardOffice.transfer(id, 5 + vm->cost(), watCard());
    } catch (VendingMachine::Stock&) {

      vm = nameServer.getMachine(id);
      printer.print(Printer::Kind::Student, id, 'V', vm->getId());
    } catch (VendingMachine::Free&) {

      isGiftCardPurchased
        ? printer.print(Printer::Kind::Student, id, 'a', favFlavour, (*giftCard).getBalance())
        : printer.print(Printer::Kind::Student, id, 'A', favFlavour, (*watCard).getBalance());
      yield(4);
      isGiftCardPurchased = false;
    } // try
  }
  delete watCard();
  if (giftCard.available()) delete giftCard();
  printer.print(Printer::Kind::Student, id, 'F');
}

