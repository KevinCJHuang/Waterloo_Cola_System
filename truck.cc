#include "truck.h"
#include "printer.h"
#include "bottlingPlant.h"
#include "nameServer.h"
#include "MPRNG.h"
#include "vendingMachine.h"

extern MPRNG mprng;
void Truck::main() {
  printer.print(Printer::Kind::Truck, 'S');
  machineList = nameServer.getMachineList();
  unsigned int cargo[numFlavours]; // soda cargo on truck
  unsigned int cargoRemaining;
  unsigned int i;                  // loop index
  unsigned int bottlesGenerated;   // # of bottles produced by plant
  unsigned int refilled;           // # of stocks refilled at vm
  unsigned int flavourEmpty;       // # of flavours with 0 soda left in cargo
  unsigned int * stock;            // stock of a vending machine
  unsigned int stockSize;
  const unsigned int stockCap = maxStockPerFlavour * numFlavours; // capacity of a vm
  for ( ;; ) {
    cargoRemaining = 0;
    stockSize = 0;
    try {
      plant.getShipment(cargo);   
      bottlesGenerated = 0;
      for (i = 0; i < numFlavours; i++)
        bottlesGenerated += cargo[i];
        cargoRemaining += cargo[i];
      printer.print(Printer::Kind::Truck, 'P', bottlesGenerated);
    } catch (BottlingPlant::Shutdown &) { // Shutdown
      break;
    }

    for (i = 0 ; i < numVendingMachines; i++) {
      printer.print(Printer::Kind::Truck, 'd', i, cargoRemaining);
      stock = machineList[curvm]->inventory();
      for (unsigned int i = 0; i < numFlavours; i++) { // refill vending machine
        refilled = stock[i] + cargo[i] > maxStockPerFlavour ? 
          (maxStockPerFlavour - stock[i]) : cargo[i];
        stock[i] += refilled;
        cargo[i] -= refilled;
        cargoRemaining -= refilled;
        stockSize += stock[i];
      }
      machineList[curvm]->restocked();

      // check whether vm is filled or not
      if (stockSize != stockCap)
        printer.print(Printer::Kind::Truck, 'U', i, stockCap - stockSize);
      
      curvm = ++curvm % numVendingMachines;
      printer.print(Printer::Kind::Truck, 'D', i, cargoRemaining);
      if (cargoRemaining == 0 ) break; // cargo is empty
    } // for


    // flat tire
    if (mprng(0,99) == 0) {
      printer.print(Printer::Kind::Truck, 'X');
      yield(10);
    }
  }

  printer.print(Printer::Kind::Truck, 'F');
}
