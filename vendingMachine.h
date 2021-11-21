#pragma once
_Monitor Printer;
_Task NameServer;
class WATCard;
_Task VendingMachine {
	unsigned int id, sodaCost;
	NameServer& nameServer;
	Printer& printer;
	void main();
	int stock [4];
  public:
		enum Flavours { BBC, CCS, RRB, JZ  }; 				// flavours of soda (YOU DEFINE)
		_Event Free {};						// free, advertisement
		_Event Funds {};					// insufficient funds
		_Event Stock {};					// flavour out of stock
		VendingMachine( Printer & prt, NameServer & nameServer, unsigned int id, unsigned int sodaCost );
		void buy( Flavours flavour, WATCard & card );
		unsigned int * inventory();
		void restocked();
		_Nomutex unsigned int cost() const;
		_Nomutex unsigned int getId() const;
};
