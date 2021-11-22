#pragma once
#include "WATCard.h"

_Monitor Bank;
_Monitor Printer;
_Task Courier;
_Task WATCardOffice {
		unsigned int numCouriers;
		Bank& bank;
		Printer& printer;
		Courier* couriers;

		// struct Job {							// marshalled arguments and return future
		// 	Args args;							// call arguments (YOU DEFINE "Args")
		// 	WATCard::FWATCard result;			// return future
		// 	Job( Args args ) : args( args ) {}
		// };
		// _Task Courier {
		// 	void main();
		// };					// communicates with bank

		void main();
  public:
		_Event Lost {};							// lost WATCard
		WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
		WATCard::FWATCard create( unsigned int sid, unsigned int amount );
		WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
		// Job * requestWork();	// Kevin: This is given, but commented to be test compiled.
};
