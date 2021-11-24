#include "WATCardOffice.h"
#include "bank.h"
#include "MPRNG.h"
#include "printer.h"

extern MPRNG mprng;
WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ): printer(prt), bank(bank), numCouriers(numCouriers) {
  couriers = new Courier [numCouriers];
  for (unsigned int i = 0; i < numCouriers; i++) {
    couriers[i].setParent(this); 
    couriers[i].setId(i);
  }
};

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  curArg = Args {sid, amount};
  curFCard = WATCard::FWATCard();
  return curFCard;
}
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
  curArg = Args {sid, amount, card};
  curFCard = WATCard::FWATCard();
  return curFCard;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
  return curJob;
}

void WATCardOffice::main() {
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
    }
  }
}

void WATCardOffice::Courier::setParent(WATCardOffice* parent) { this->parent = parent; }
void WATCardOffice::Courier::setId(unsigned int id) { this->id = id; }

void WATCardOffice::Courier::main() {
  _Accept (setParent);
  _Accept (setId);
  parent->printer.print(Printer::Kind::Courier, id, 'S');
  WATCardOffice::Job* job;
  for ( ;; ) {
    job = parent->requestWork();
    parent->printer.print(Printer::Kind::Courier, 't', job->args.sid, job->args.amount);
    parent->bank.withdraw( job->args.sid, job->args.amount );
    job->args.card->deposit(job->args.amount);

    if (mprng (5) == 0) {
      job->result.exception(new WATCardOffice::Lost()); // Lost
      parent->printer.print(Printer::Kind::Courier, id, 'L', job->args.sid);
    } else {
      job->result.delivery(job->args.card);            // delivered
      parent->printer.print(Printer::Kind::Courier, id, 'T', job->args.sid, job->args.amount);
    }
    delete job;
  }
  parent->printer.print(Printer::Kind::Courier, id, 'F');
}
