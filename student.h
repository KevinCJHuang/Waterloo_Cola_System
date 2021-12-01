#pragma once
_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Groupoff;
#include "WATCard.h"
#include "vendingMachine.h"
_Task Student {
		Printer& printer;
		NameServer& nameServer;
		WATCardOffice& cardOffice;
		Groupoff& groupoff;
		unsigned int id, numPurchases;
		unsigned int maxPurchases;
		WATCard::FWATCard giftCard, watCard;
		VendingMachine::Flavours favFlavour;
		VendingMachine*  vm;
		void main();
  public:
		Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases );
};
