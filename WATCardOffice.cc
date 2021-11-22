#include "WATCardOffice.h"

WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ): printer(prt), bank(bank), numCouriers(numCouriers) {
  couriers = new Courier [numCouriers];
};

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  return FWATCard();
}
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card );

voig WATCardOffice::main() {

}
