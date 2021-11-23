#include "student.h"
#include "MPRNG.h"
#include "vendingMachine.h"  // access: Flavours
#include "nameServer.h"
#include "WATCardOffice.h"
#include "groupOff.h"

extern MPRNG mprng;

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases )
: printer(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff),
id(id), maxPurchases(maxPurchases) {}


void Student::main() {
  unsigned int numPurchases = mprng(1, maxPurchases);
  unsigned int purchased = 0;
  VendingMachine::Flavours favFlavour = 
    static_cast<VendingMachine::Flavours>(mprng(0, 3));
  VendingMachine* vm = nameServer.getMachine(id);
  
  WATCard::FWATCard watCard = cardOffice.create(id, 5);
  WATCard::FWATCard giftCard = groupoff.giftCard();
  
  for ( ;; ) {
  if (purchased == numPurchases) break;
    yield(mprng(1, 10));



    try {
      if (giftCard.available()) {
        vm->buy( favFlavour, *giftCard() );
        giftCard.reset();
      } else if (watCard.available()) {
        vm->buy( favFlavour, *watCard() );
      } else { // none of them are available; needs to wait
        _Select (giftCard) {
          vm->buy( favFlavour, *giftCard() );
          giftCard.reset();
        }
        or _Select (watCard) {
          vm->buy( favFlavour, *watCard() );
        }
      }

      purchased++;
    } catch (WATCardOffice::Lost&) {
      watCard = cardOffice.create(id, 5);
    } catch (VendingMachine::Funds&) {
      watCard = cardOffice.transfer(id, 5 + vm->cost(), watCard);
    } catch (VendingMachine::Stock&) {
      vm = nameServer.getMachine(id);
    } catch (VendingMachine::Free&) {
      yield(4);
    } // try
  }
}
