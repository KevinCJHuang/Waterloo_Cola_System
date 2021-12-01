#include "WATCardOffice.h"
#include "bank.h"
#include "MPRNG.h"
#include "printer.h"

extern MPRNG mprng;
WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ): printer(prt), bank(bank), numCouriers(numCouriers) {
  couriers = new Courier* [numCouriers];
  for (unsigned int i = 0; i < numCouriers; i++) {
    couriers[i] = new Courier (printer, i, this);
  }
};

WATCardOffice::~WATCardOffice() {

}
WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
  curArg = Args {sid, amount};
  curFCard = WATCard::FWATCard();
  return curFCard;
}
WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard * card ) {
  curArg = Args {sid, amount, card};
  curFCard = WATCard::FWATCard();
  printer.print(Printer::Kind::WATCardOffice, 'T', sid, amount);
  return curFCard;
}

WATCardOffice::Job * WATCardOffice::requestWork() {
  if (jobs.size() == 0) jobBench.wait();
  Job* rv = jobs.front();
  jobs.pop();
  if (!(rv->args.end)) printer.print(Printer::Kind::WATCardOffice, 'W');
  return rv;
}


void WATCardOffice::main() {
  printer.print(Printer::Kind::WATCardOffice, 'S');
  Courier* couriers [numCouriers];

  for ( ;; ) {
    _Accept (requestWork) {}
    or _Accept (create) {
      curArg.card = new WATCard(); // Create new card for courier
      Job* newJob = new Job (curArg);
      newJob->result = curFCard;
      jobs.push(newJob);
      printer.print(Printer::Kind::WATCardOffice, 'C', curArg.sid, curArg.amount);
      jobBench.signalBlock();
    } or _Accept (transfer) {
      Job* newJob = new Job (curArg);
      newJob->result = curFCard;
      jobs.push(newJob);
      printer.print(Printer::Kind::WATCardOffice, 'C', curArg.sid, curArg.amount);
      jobBench.signalBlock();
    } or _Accept (~WATCardOffice) {
      // clear existing work
      Job newJob (Args{0,0,nullptr, true});   // Create new job
      for (unsigned int i = 0; i < numCouriers; i++) {
        jobs.push(&newJob);
      }
      unsigned int unfinishedCouriers = numCouriers;
      while (!jobBench.empty()) {
        jobBench.signalBlock();
        unfinishedCouriers--;
      }
      for ( ;; ) {
      if (unfinishedCouriers == 0) break;
        _Accept (requestWork);
        unfinishedCouriers--;
      }
      for (unsigned int i = 0; i < numCouriers; i++) {
        delete couriers[i];
      }
      break;
    }
  }
  printer.print(Printer::Kind::WATCardOffice, 'F');
}

void WATCardOffice::Courier::main() {
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
      delete job->args.card;
      printer.print(Printer::Kind::Courier, id, 'L', job->args.sid);
    } else {
      job->result.delivery(job->args.card);            // delivered
      printer.print(Printer::Kind::Courier, id, 'T', job->args.sid, job->args.amount);
    }
    delete job;
  }
  printer.print(Printer::Kind::Courier, id, 'F');
}
