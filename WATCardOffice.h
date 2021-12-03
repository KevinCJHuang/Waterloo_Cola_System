#pragma once
#include "WATCard.h"
#include <queue>

_Monitor Bank;
_Monitor Printer;
_Task Courier;
_Task WATCardOffice {
		// given by ctor
		unsigned int numCouriers;
		Bank& bank;
		Printer& printer;

		struct Args {
			unsigned int sid;
			unsigned int amount;
			WATCard* card = nullptr;
		};

		struct Job { // marshalled arguments and return future
			Args args; // call arguments (YOU DEFINE "Args")
			WATCard::FWATCard result;			// return future
			Job( Args args ) : args( args ) {}
		};

		_Task Courier {
				Printer& printer;
				WATCardOffice* parent;
				unsigned int id;
				void main();
			public:
				Courier (Printer& printer, unsigned int id, WATCardOffice* parent)
					: id(id), parent(parent), printer(printer) {}
		};					// communicates with bank

		bool isTerminate = false;			// flag for terminating couriers
		_Event Terminate {};					// raised at each courier for termination
		Args curArg;									// current arg from transfer/create calls
		WATCard::FWATCard curFCard;		// current FWATCard from transfer/create calls
		std::queue<Job*> jobs; 				// queue of jobs to be done by courier
 		uCondition jobBench;					// when there's no job, couriers sit on this bench
		void createJob(char state);		// create & push a new job; state = 'T' or 'C'
		void main();
  public:
		_Event Lost {};							// lost WATCard
		WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers );
		~WATCardOffice();
		WATCard::FWATCard create( unsigned int sid, unsigned int amount );
		WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard * card );
		Job * requestWork();
};
