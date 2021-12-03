#include "truck.h"
#include "printer.h"
#include "bottlingPlant.h"
#include "nameServer.h"
#include "MPRNG.h"
#include "vendingMachine.h"
using namespace std; // DEBUG
extern MPRNG mprng;
void Truck::main() {
  machineList = nameServer.getMachineList();
  printer.print(Printer::Kind::Truck, 'S');

  unsigned int i;                  // loop index
  unsigned int cargo[numFlavours]; // soda cargo on truck
  unsigned int cargoCount;         // Remaining cargo in the truck
  unsigned int refilledCount;      // # of stocks refilledCount at vm
  unsigned int * stock;            // stock of a vending machine
  unsigned int stockSize;          // stock size of a vending machine
  const unsigned int stockCap = maxStockPerFlavour * numFlavours; // capacity of a vm

  for ( ;; ) {
    yield(mprng(1, 10));
    try {
      _Enable{
        plant.getShipment(cargo);
      }
    } catch (BottlingPlant::Shutdown &) { break; }

    cargoCount = 0;
    for (i = 0; i < numFlavours; i++) {
      cargoCount += cargo[i];
    }
    printer.print(Printer::Kind::Truck, 'P', cargoCount);

    for (i = 0 ; i < numVendingMachines; i++) {
      printer.print(Printer::Kind::Truck, 'd', machineList[curvm]->getId(), cargoCount);
      
      // Fetch vm's stock and refill the vm
      stock = machineList[curvm]->inventory();
      stockSize = 0;
      for (unsigned int i = 0; i < numFlavours; i++) { 
        refilledCount = stock[i] + cargo[i] > maxStockPerFlavour ? 
          (maxStockPerFlavour - stock[i]) : cargo[i];
        stock[i] += refilledCount;
        cargo[i] -= refilledCount;
        cargoCount -= refilledCount;
        stockSize += stock[i];
      }
      machineList[curvm]->restocked();

      // check whether vm is filled or not
      if (stockSize != stockCap) {
        printer.print(Printer::Kind::Truck, 'U', machineList[curvm]->getId(), stockCap - stockSize);
      }

      printer.print(Printer::Kind::Truck, 'D', machineList[curvm]->getId(), cargoCount);
      curvm = ++curvm % numVendingMachines;
      
      // flat tire
      if (mprng(0,99) == 0) {
        printer.print(Printer::Kind::Truck, 'X');
        yield(10);
      }

      if (cargoCount == 0 ) break; // cargo is empty
    } // for
  }
  printer.print(Printer::Kind::Truck, 'F');
}
