#include "WATCardOffice.h"
#include "bank.h"
#include "MPRNG.h"
#include "printer.h"

extern MPRNG mprng;
WATCardOffice::WATCardOffice( Printer & prt, Bank & bank, unsigned int numCouriers ): printer(prt), bank(bank), numCouriers(numCouriers) {

};

WATCardOffice::~WATCardOffice() { }
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
  if (jobs.size() == 0) jobBench.wait(); // wait if there's no job to do
  if (isTerminate) _Throw Terminate{};   // terminate courier
  Job* rv = jobs.front();                // take first job in the job queue
  jobs.pop();                            // pop it from the job queue
  printer.print(Printer::Kind::WATCardOffice, 'W');
  return rv;
}

void WATCardOffice::createJob(char state) {
  Job* newJob = new Job (curArg); // create new job using curArg
  newJob->result = curFCard;      // and curFCard
  jobs.push(newJob);              // then push it into job queue
  printer.print(Printer::Kind::WATCardOffice, state, curArg.sid, curArg.amount);
  jobBench.signalBlock();         // if there's any worker on the bench, wake them up
}

void WATCardOffice::main() {
  printer.print(Printer::Kind::WATCardOffice, 'S');

  // create couriers
  Courier* couriers [numCouriers];
  for (unsigned int i = 0; i < numCouriers; i++) {
    couriers[i] = new Courier (printer, i, this);
  }
  for ( ;; ) {
    _Accept (requestWork) {
    } or _Accept (create) {
      curArg.card = new WATCard(); // create new card for courier
      createJob('C');              // create new job & push to job queue
    } or _Accept (transfer) {
      createJob('T');              // create new job & push to job queue
    } or _Accept (~WATCardOffice) {
      isTerminate = true;
      unsigned int unfinishedCouriers = numCouriers;

      // Wake up & terminate couriers waiting on the bench
      while (!jobBench.empty()) {
        jobBench.signalBlock();
        unfinishedCouriers--;
      }

      // accept remaining couriers, and terminate them
      for (unsigned int i = 0; i < unfinishedCouriers; i++) {
        _Accept (requestWork);
        jobBench.signalBlock();
      }

      // memory manage
      for (unsigned int i = 0; i < numCouriers; i++) {
        delete couriers[i];
      }
      break;
    } // _Accept
  } // for
  printer.print(Printer::Kind::WATCardOffice, 'F');
}

void WATCardOffice::Courier::main() {
  printer.print(Printer::Kind::Courier, id, 'S');
  WATCardOffice::Job* job;
  for ( ;; ) {
    try {
      job = parent->requestWork();  // request work
    } catch ( WATCardOffice::Terminate& ) { break; } // if raised Terminate(), break

    printer.print(Printer::Kind::Courier, id, 't', job->args.sid, job->args.amount);
    parent->bank.withdraw( job->args.sid, job->args.amount );
    job->args.card->deposit(job->args.amount);

    if (mprng (5) == 0) {
      job->result.exception(new WATCardOffice::Lost()); // Lost
      delete job->args.card; // delete card
      printer.print(Printer::Kind::Courier, id, 'L', job->args.sid);
    } else {
      // card's ownership/memory will be managed by student
      job->result.delivery(job->args.card);            // delivered
      printer.print(Printer::Kind::Courier, id, 'T', job->args.sid, job->args.amount);
    }
    delete job;
  }
  printer.print(Printer::Kind::Courier, id, 'F');
}
