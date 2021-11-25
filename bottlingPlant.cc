#include "bottlingPlant.h"
#include "printer.h"
#include "nameServer.h"
#include "MPRNG.h"
extern MPRNG mprng;

void BottlingPlant::main() {
  Truck truck (printer, nameServer, *this, numVendingMachines, maxStockPerFlavour);
  yield(timeBetweenShipments); // yield before first shipment
  printer.print(Printer::Kind::BottlingPlant, 'S');

  unsigned int generatedBottles;

  for ( ;; ) {
  if (isShutdown) break;
    generatedBottles=0;
    // Perform a production run
    for (unsigned int i = 0; i < 4; i++) {
      stock[i] = mprng(0, maxShippedPerFlavour);
      generatedBottles += stock[i];
    }

    printer.print(Printer::Kind::BottlingPlant, 'G', generatedBottles);

    try {
      _Accept (getShipment) {}
      or _Accept (~BottlingPlant) {
        isShutdown = true;
        _Accept (getShipment)
      }
    } catch (uMutexFailure::RendezvousFailure &) { // Shutdown
      printer.print(Printer::Kind::BottlingPlant, 'F');
      break;
    }
    yield(timeBetweenShipments); // yield before each shipment
  }
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
  if (isShutdown) throw Shutdown{};
  printer.print(Printer::Kind::BottlingPlant, 'P');
  for (unsigned int i = 0; i < 4; i++) {
    cargo[i] = stock[i];
  }
}
