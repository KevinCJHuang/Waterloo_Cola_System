#pragma once
_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Groupoff;
#include "vendingMachine.h"  // access: Flavours
#include "WATCard.h"

_Task Student {
		Printer& printer;
		NameServer& nameServer;
		WATCardOffice& cardOffice;
		Groupoff& groupoff;
		unsigned int id, numPurchases;
		VendingMachine::Flavours favFlavour;
		WATCard::FWATCard watCard;
		WATCard::FWATCard giftCard;

		void main();
  public:
		Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases );
};
