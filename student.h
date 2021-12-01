#pragma once
_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Groupoff;
#include "WATCard.h"
_Task Student {
		Printer& printer;
		NameServer& nameServer;
		WATCardOffice& cardOffice;
		Groupoff& groupoff;
		unsigned int id, numPurchases;

		void main();
  public:
		Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice,
		Groupoff & groupoff, unsigned int id, unsigned int maxPurchases );
};
