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
  if (isShutdown) break;
    yield(timeBetweenShipments); // yield before each shipment; order could be changed!!!!!!!!!!!!!!!!!!

    generatedBottles=0;
    // Perform a production run
    for (unsigned int i = 0; i < 4; i++) {
      stock[i] = mprng(0, maxShippedPerFlavour);
      generatedBottles += stock[i];
    }
    cout << endl << "plant bottles generated" << endl;

    printer.print(Printer::Kind::BottlingPlant, 'G', generatedBottles);

    _Accept (getShipment) {}
    or _Accept (~BottlingPlant) {
      isShutdown = true;
      try {
        _Accept (getShipment)
      } catch (uMutexFailure::RendezvousFailure &) { }// Shutdown
      break;
    }
  }
  printer.print(Printer::Kind::BottlingPlant, 'F');
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
  if (isShutdown) throw Shutdown{};
  printer.print(Printer::Kind::BottlingPlant, 'P');
  for (unsigned int i = 0; i < 4; i++) {
    cargo[i] = stock[i];
  }
}
