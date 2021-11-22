#include "student.h"
#include "MPRNG.h"
extern MPRNG mprng;

Student::Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases )
: printer(prt), nameServer(nameServer), cardOffice(cardOffice), groupoff(groupoff),
id(id) {}


void Student::main() {
  numPurchases = mprng(1, maxPurchases);
  favFlavour = mprng(0, 3);
  
  card = cardOffice.create(id, 5);
  giftCard = groupoff

  for ( ;; ) {
    
  }

}