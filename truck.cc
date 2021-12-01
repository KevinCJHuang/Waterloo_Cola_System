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

  unsigned int cargo[numFlavours]; // soda cargo on truck
  unsigned int cargoRemaining;
  unsigned int i;                  // loop index
  unsigned int refilled;           // # of stocks refilled at vm
  unsigned int flavourEmpty;       // # of flavours with 0 soda left in cargo
  unsigned int * stock;            // stock of a vending machine
  unsigned int stockSize;
  const unsigned int stockCap = maxStockPerFlavour * numFlavours; // capacity of a vm
  // cout << endl << "stockCap: " <<  stockCap << endl;

  for ( ;; ) {
    cargoRemaining = 0;
    try {
      #ifdef DEBUG
      cout << "truck yielding" << endl;
      #endif
      yield(mprng(1, 10));
      plant.getShipment(cargo);
      for (i = 0; i < numFlavours; i++) {
        cargoRemaining += cargo[i];
      }
      printer.print(Printer::Kind::Truck, 'P', cargoRemaining);
    } catch (BottlingPlant::Shutdown &) { // Shutdown
      break;
    }

    for (i = 0 ; i < numVendingMachines; i++) {
      stockSize = 0;
      printer.print(Printer::Kind::Truck, 'd', machineList[curvm]->getId(), cargoRemaining);
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
      if (stockSize != stockCap) {
        printer.print(Printer::Kind::Truck, 'U', machineList[curvm]->getId(), stockCap - stockSize);
      }

      printer.print(Printer::Kind::Truck, 'D', machineList[curvm]->getId(), cargoRemaining);
      curvm = ++curvm % numVendingMachines;
      
      // flat tire
      if (mprng(0,99) == 0) {
        printer.print(Printer::Kind::Truck, 'X');
        yield(10);
      }
      #ifdef DEBUG
      cout << endl << "truck flattire mprng" << endl;
      #endif
      if (cargoRemaining == 0 ) break; // cargo is empty
    } // for



  }
  printer.print(Printer::Kind::Truck, 'F');
}
