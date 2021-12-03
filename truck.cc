#include "truck.h"
#include "printer.h"
#include "bottlingPlant.h"
#include "nameServer.h"
#include "MPRNG.h"
#include "vendingMachine.h"

extern MPRNG mprng;
void Truck::main() {
  printer.print(Printer::Kind::Truck, 'S');
  machineList = nameServer.getMachineList();  // obtain location of vm

  // Initialize variables
  unsigned int i;                  // loop index
  unsigned int cargo[numFlavours]; // soda cargo on truck
  unsigned int cargoCount;         // Remaining cargo in the truck
  unsigned int refilledCount;      // # of stocks refilledCount at vm
  unsigned int * stock;            // array; stock of a vending machine
  unsigned int stockSize;          // stock size of a vending machine
  const unsigned int stockCap = maxStockPerFlavour * numFlavours; // capacity of a vm

  // starts delivery runs
  for ( ;; ) {
    yield(mprng(1, 10)); // to get a coffee
    try {
      _Enable {
        plant.getShipment(cargo); // to get shipments from the plant
      }
    } catch (BottlingPlant::Shutdown &) { break; }

    // count cargo received from the plant
    cargoCount = 0;
    for (i = 0; i < numFlavours; i++) cargoCount += cargo[i];
    printer.print(Printer::Kind::Truck, 'P', cargoCount);

    // deliver cargo
    for (i = 0 ; i < numVendingMachines; i++) {
      printer.print(Printer::Kind::Truck, 'd', machineList[curvm]->getId(), cargoCount);
      
      // fetch vm's stock and refill the vm
      stock = machineList[curvm]->inventory();
      stockSize = 0;
      for (unsigned int i = 0; i < numFlavours; i++) { // for each flavour
        // calculate refillable amount
        refilledCount = stock[i] + cargo[i] > maxStockPerFlavour ? 
          (maxStockPerFlavour - stock[i]) : cargo[i];
        stock[i] += refilledCount;    // refill the vm
        cargo[i] -= refilledCount;    // remove cargo
        cargoCount -= refilledCount;  // decrease cargo count
        stockSize += stock[i];        // increase stockSize
      }
      machineList[curvm]->restocked();

      // check whether vm is filled or not
      if (stockSize != stockCap) {
        printer.print(Printer::Kind::Truck, 'U', machineList[curvm]->getId(), stockCap - stockSize);
      }

      printer.print(Printer::Kind::Truck, 'D', machineList[curvm]->getId(), cargoCount);
      curvm = ++curvm % numVendingMachines; // update next vm to visit
      
      // flat tire
      if (mprng(0,99) == 0) {
        printer.print(Printer::Kind::Truck, 'X');
        yield(10);
      }

      if (cargoCount == 0 ) break; // cargo is empty
    } // for
  } // for
  printer.print(Printer::Kind::Truck, 'F');
}
