#include "truck.h"
#include "printer.h"
#include "bottlingPlant.h"
#include "nameServer.h"
#include "MPRNG.h"
#include "vendingMachine.h"

extern MPRNG mprng;
void Truck::main() {
  machineList = nameServer.getMachineList();
  printer.print(Printer::Kind::Truck, 'S');

  unsigned int cargo[numFlavours]; // soda cargo on truck
  unsigned int cargoRemaining;
  unsigned int i;                  // loop index
  unsigned int bottlesGenerated;   // # of bottles produced by plant
  unsigned int refilled;           // # of stocks refilled at vm
  unsigned int flavourEmpty;       // # of flavours with 0 soda left in cargo
  unsigned int * stock;            // stock of a vending machine
  unsigned int stockSize;
  const unsigned int stockCap = maxStockPerFlavour * numFlavours; // capacity of a vm
  // cout << endl << "stockCap: " <<  stockCap << endl;

  for ( ;; ) {
    _Accept(~Truck) {break;} _Else
    cargoRemaining = 0;
    try {
      plant.getShipment(cargo);
      // cout << endl << "cargo begin: " <<  cargo[0] << ", "<< cargo[1] << ", "<< cargo[2] << ", "<< cargo[3] << endl;
      bottlesGenerated = 0;
      for (i = 0; i < numFlavours; i++) {
        bottlesGenerated += cargo[i];
        cargoRemaining += cargo[i];
      }
      // cout << endl << "truck" << bottlesGenerated<< endl;
      printer.print(Printer::Kind::Truck, 'P', bottlesGenerated);
    } catch (BottlingPlant::Shutdown &) { // Shutdown
      break;
    }

    for (i = 0 ; i < numVendingMachines; i++) {
      stockSize = 0;
      printer.print(Printer::Kind::Truck, 'd', curvm, cargoRemaining);
      stock = machineList[curvm]->inventory();
      // cout << "stock: " << stock[0] << endl;
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

        printer.print(Printer::Kind::Truck, 'U', curvm, stockCap - stockSize);

      }
      
      // curvm ++;
      // curvm %= numVendingMachines;
      printer.print(Printer::Kind::Truck, 'D', curvm, cargoRemaining);
      curvm = ++curvm % numVendingMachines;
      if (cargoRemaining == 0 ) break; // cargo is empty
    } // for


    // flat tire
    if (mprng(0,99) == 0) {
      printer.print(Printer::Kind::Truck, 'X');
      yield(10);
    }
    cout << endl << "truck flattire mprng" << endl;

  }
  printer.print(Printer::Kind::Truck, 'F');
}
