#include "bottlingPlant.h"
#include "printer.h"
#include "nameServer.h"
#include "MPRNG.h"
extern MPRNG mprng;

BottlingPlant::BottlingPlant( Printer & prt, NameServer & nameServer,
  unsigned int numVendingMachines, unsigned int maxShippedPerFlavour,
  unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments )
  : numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
  maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments),
  printer(prt), nameServer(nameServer) {};

void BottlingPlant::main() {
  printer.print(Printer::Kind::BottlingPlant, 'S');

  // create truck
  Truck truck (printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);

  unsigned int generatedBottles; // # of bottles generated per production run

  for ( ;; ) {
    yield(timeBetweenShipments); // yield between shipments
    
    // Perform a production run
    generatedBottles=0;
    for (unsigned int i = 0; i < 4; i++) {
      stock[i] = mprng(maxShippedPerFlavour); // Generate random # of bottles per flavour 
      generatedBottles += stock[i];
    }
    printer.print(Printer::Kind::BottlingPlant, 'G', generatedBottles);

    _Accept (~BottlingPlant) { // If dtor is called, shutdown truck & self
      isShutdown = true;       // flag variable for getShipment()
      _Accept (getShipment)    // raise Shutdown{} on truck
      break;
    }
    or _Accept (getShipment) {
      printer.print(Printer::Kind::BottlingPlant, 'P');
    }
  }
  printer.print(Printer::Kind::BottlingPlant, 'F');
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
  if (isShutdown) {
    _Resume Shutdown{} _At uThisTask();   // Terminate the truck
  }
  for (unsigned int i = 0; i < 4; i++) {  // fill truck's carge
    cargo[i] = stock[i];
  }
}
