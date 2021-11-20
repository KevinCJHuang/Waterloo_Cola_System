#pragma once
_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Groupoff;

_Task Student {
	void main();
  public:
	Student( Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice, Groupoff & groupoff, unsigned int id, unsigned int maxPurchases );
};
