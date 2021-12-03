#pragma once
_Monitor Printer;
_Task NameServer;
class WATCard;
_Task VendingMachine {
	public:
		enum Flavours { BBC, CCS, RRB, JZ  }; 				// flavours of soda (YOU DEFINE)
	private:
		unsigned int id, sodaCost;
		NameServer& nameServer;
		Printer& printer;
		Flavours curFlavour;  // Flavour of last purchase
		enum PurchaseState { FREE, STOCK, FUNDS, BUY };
		uCondition bench;	  // for VendingMachine::buy()
		WATCard* curCard;	  // for VendingMachine::buy()
		PurchaseState state;  // for VendingMachine::buy()
		unsigned int stock [4] {0,0,0,0};
 		void main();

  public:
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
