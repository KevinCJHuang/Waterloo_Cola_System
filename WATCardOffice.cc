#include "WATCardOffice.h"
#include "bank.h"
#include "MPRNG.h"
#include "printer.h"

extern MPRNG mprng;
WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ): printer(prt), bank(bank), numCouriers(numCouriers) {
  couriers = new Courier* [numCouriers];
  // Courier* couriers [numCouriers];
  for (unsigned int i = 0; i < numCouriers; i++) {
    couriers[i] = new Courier (printer, i, this);
  }
};

WATCardOffice::~WATCardOffice() {

}
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  curArg = Args {sid, amount};
  curFCard = WATCard::FWATCard();
  printer.print(Printer::Kind::WATCardOffice, 'C', sid, amount);
  return curFCard;
}
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
  curArg = Args {sid, amount, card};
  curFCard = WATCard::FWATCard();
  printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
  return curFCard;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
  if (!(curJob->args.end)) printer.print(Printer::Kind::WATCardOffice, 'W');
  return curJob;
}

void WATCardOffice::main() {
  printer.print(Printer::Kind::WATCardOffice, 'S');

  // cout << "watcardOff: courisers created " << endl;

  for ( ;; ) {
    _Accept (create) {
      curArg.card = new WATCard(); // Create new card for courier
      curJob = new Job (curArg);   // Create new job
      curJob->result = curFCard;    // Set new job's FWATCard result
      _Accept(requestWork);        // Let courier do the work
    } or _Accept (transfer) {
      curJob = new Job (curArg);   // Create new job
      curJob->result = curFCard;    // Set new job's FWATCard result
      _Accept(requestWork);        // Let courier do the work
    } or _Accept (~WATCardOffice) {
      Job curJob (Args{0,0,nullptr, true});   // Create new job
      for (unsigned int i = 0; i < numCouriers; i++) {
        _Accept(requestWork);        // Let courier do the work
      }
      for (int unsigned i = 0; i < numCouriers; i++) {
        delete couriers[i];
      }
      break;
    }
  }

  printer.print(Printer::Kind::WATCardOffice, 'F');
}

void WATCardOffice::Courier::main() {
  // cout << "courier printing: " << endl;
  printer.print(Printer::Kind::Courier, id, 'S');
  WATCardOffice::Job* job;
  for ( ;; ) {
    job = parent->requestWork();
    if (job->args.end) break;
    printer.print(Printer::Kind::Courier, id, 't', job->args.sid, job->args.amount);
    parent->bank.withdraw( job->args.sid, job->args.amount );
    job->args.card->deposit(job->args.amount);

    if (mprng (5) == 0) {
      job->result.exception(new WATCardOffice::Lost()); // Lost
      printer.print(Printer::Kind::Courier, id, 'L', job->args.sid);
    } else {
      job->result.delivery(job->args.card);            // delivered
      printer.print(Printer::Kind::Courier, id, 'T', job->args.sid, job->args.amount);
    }
    delete job;
  }
  printer.print(Printer::Kind::Courier, id, 'F');
}
