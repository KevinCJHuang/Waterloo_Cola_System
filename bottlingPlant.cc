#include "bottlingPlant.h"
#include "printer.h"
#include "nameServer.h"
#include "MPRNG.h"
extern MPRNG mprng;

void BottlingPlant::main() {
  printer.print(Printer::Kind::BottlingPlant, 'S');
  Truck truck (printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);

  unsigned int generatedBottles;

  for ( ;; ) {
    yield(timeBetweenShipments); // yield before each shipment; order could be changed!!!!!!!!!!!!!!!!!!
    generatedBottles=0;
    // Perform a production run
    for (unsigned int i = 0; i < 4; i++) {
      stock[i] = mprng(0, maxShippedPerFlavour);
    #ifdef DEBUG
      cout << endl << "plant bottles generated - " << i << endl;
    #endif
      generatedBottles += stock[i];
    }

    printer.print(Printer::Kind::BottlingPlant, 'G', generatedBottles);

    _Accept (~BottlingPlant) {
      isShutdown = true;
      _Accept (getShipment)
      break;
    }
    or _Accept (getShipment) {
      printer.print(Printer::Kind::BottlingPlant, 'P');
    }
  }
  printer.print(Printer::Kind::BottlingPlant, 'F');
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
  if (isShutdown) _Resume Shutdown{} _At uThisTask();
  for (unsigned int i = 0; i < 4; i++) { 
    cargo[i] = stock[i];
  }
}
