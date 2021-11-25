#pragma once
#include "WATCard.h"

_Monitor Bank;
_Monitor Printer;
_Task Courier;
_Task WATCardOffice {
		struct Args {
			unsigned int sid;
			unsigned int amount;
			WATCard* card = nullptr;
			bool end = false;
		};

		struct Job { // marshalled arguments and return future
			Args args; // call arguments (YOU DEFINE "Args")
			WATCard::FWATCard result;			// return future
			Job( Args args ) : args( args ) {}
		};
		_Task Courier {
				WATCardOffice* parent;
				unsigned int id;
				void main();
			public:
				void setParent(WATCardOffice* parent);
				void setId(unsigned int id);
		};					// communicates with bank


		unsigned int numCouriers;
		Bank& bank;
		Printer& printer;
		Courier* couriers;
		Args curArg;
		// unsigned int curId;
		WATCard::FWATCard curFCard;
		// unsigned int curAmount;
		Job* curJob;
		

		void main();
  public:
		_Event Lost {};							// lost WATCard
		WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
		~WATCardOffice();
		WATCard::FWATCard create( unsigned int sid, unsigned int amount );
		WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
		Job * requestWork();
};
