#include "student.h"
#include "MPRNG.h"
#include "nameServer.h"
#include "WATCardOffice.h"
#include "groupOff.h"
#include "printer.h"
#include "vendingMachine.h"

extern MPRNG mprng;

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice,
  Groupoff & groupoff, unsigned int id, unsigned int maxPurchases ): printer(prt),
  nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff), id(id) {
    numPurchases = mprng(1, maxPurchases);
}

void Student::main() {
  // choose favouriate flavour
  VendingMachine::Flavours favFlavour
    = static_cast<VendingMachine::Flavours>(mprng(3));
  printer.print(Printer::Kind::Student, id, 'S', favFlavour, numPurchases);

  // create gift card & watcard
  WATCard::FWATCard watCard = cardOffice.create(id, 5);
  WATCard::FWATCard giftCard = groupoff.giftCard();

  // obtain location of vm to visit
  VendingMachine* vm = nameServer.getMachine(id);
  printer.print(Printer::Kind::Student, id, 'V', vm->getId());

  // start purchases
  bool giftCardPurchase;         // track card used for VendingMachine::Free print msg
  yield(mprng(1, 10));           // yiled before first purchase
  for (unsigned int purchased = 0; purchased < numPurchases; ) {
    try {
      _Select (giftCard) {       // Wait for gift card
        giftCardPurchase = true;
        vm->buy( favFlavour, *giftCard ); // make the purchase
        printer.print(Printer::Kind::Student, id, 'G', favFlavour, (*giftCard).getBalance());
        delete giftCard;
        giftCard.reset();        // Discard gift card
      } or _Select (watCard) {     // Wait for WATCard
        giftCardPurchase = false;
        vm->buy( favFlavour, *watCard ); // make the purchase
        printer.print(Printer::Kind::Student, id, 'B', favFlavour, (*watCard).getBalance());
      }
      purchased++;
    } catch (WATCardOffice::Lost&) {
      printer.print(Printer::Kind::Student, id, 'L');
      watCard = cardOffice.create(id, 5);                           // create a new card
      continue; // to skip yield; Lost doesn't need to yield
    } catch (VendingMachine::Funds&) {
      watCard = cardOffice.transfer(id, 5 + vm->cost(), watCard()); // send courier to get more money
    } catch (VendingMachine::Stock&) {
      vm = nameServer.getMachine(id);
      printer.print(Printer::Kind::Student, id, 'V', vm->getId());
    } catch (VendingMachine::Free&) {
      giftCardPurchase
        ? printer.print(Printer::Kind::Student, id, 'a', favFlavour, (*giftCard).getBalance())
        : printer.print(Printer::Kind::Student, id, 'A', favFlavour, (*watCard).getBalance());
      yield(4);
      continue; // to skip yield; Free doesn't need to yield
    } // try
    yield(mprng(1, 10)); // yiled before next purchase
  } // for
  delete watCard(); // delete watCard, no matter it's used or not
  if (giftCard.available()) delete giftCard(); // delete gift card if it is not used
  printer.print(Printer::Kind::Student, id, 'F');
}

