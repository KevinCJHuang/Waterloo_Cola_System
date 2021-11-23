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
  unsigned int i;                  // loop index
  unsigned int bottlesGenerated;   // # of bottles produced by plant
  unsigned int refilled;           // # of stocks refilled at vm
  unsigned int flavourEmpty;       // # of flavours with 0 soda left in cargo
  unsigned int * stock;            // stock of a vending machine
  for ( ;; ) {
    try {
      plant.getShipment(cargo);   
      bottlesGenerated = 0;
      for (i = 0; i < numFlavours; i++)
        bottlesGenerated += cargo[i];
      printer.print(Printer::Kind::Truck, 'P', bottlesGenerated);
    } catch (BottlingPlant::Shutdown &) { // Shutdown
      break;
    }

    DELIVERY: for (i = 0 ; i < numVendingMachines; i++) {
      printer.print(Printer::Kind::Truck, 'd', i, bottlesGenerated);
      stock = machineList[curvm]->inventory();
      flavourEmpty = 0;
      for (unsigned int i = 0; i < numFlavours; i++) {
        refilled = stock[i] + cargo[i] > maxStockPerFlavour ? 
          (maxStockPerFlavour - stock[i]) : cargo[i];
        stock[i] += refilled;
        cargo[i] -= refilled;
        if (cargo[i] == 0) flavourEmpty++;
      }
      machineList[curvm]->restocked();

      int notReplenished = 0; // to find the total number of bottles not replenished
      for (int j = 0; j < numFlavours; j++)
        notReplenished += cargo[j];
      printer.print(Printer::Kind::Truck, 'U', i, notReplenished);
      
      curvm += 1;
      curvm %= numVendingMachines;
      printer.print(Printer::Kind::Truck, 'D', i, cargo[i]);
      if (flavourEmpty == numFlavours) break DELIVERY; // cargo is empty
    } // DELIVERY

    // flat tire
    if (mprng(0,99) == 0) {
      printer.print(Printer::Kind::Truck, 'X');
      yield(10);
    }
  }

  printer.print(Printer::Kind::Truck, 'F');
}
