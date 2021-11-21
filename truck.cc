#include "truck.h"
#include "printer.h"
#include "bottlingPlant.h"
#include "nameServer.h"
#include "MPRNG.h"
extern MPRNG mprng;
void Truck::main() {
  printer.print(Printer::Kind::Truck, 'S');
  machineList = nameServer.getMachineList();
  unsigned int cargo[numFlavours];
  unsigned int i, bottlesGenerated;

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
      // numVendingMachines[curvm].restock? Fix!!!!!!!!!!!!!!!
      for (i = 0; i < numFlavours; i++) {
        if (cargo[i] == 0) break DELIVERY;
      } // for
      curvm += 1;
      curvm %= numVendingMachines;
    } // DELIVERY

    // flat tire
    if (mprng(0,99) == 0) {
      printer.print(Printer::Kind::Truck, 'X');
      yield(10);
    }
  }

  printer.print(Printer::Kind::Truck, 'F');
}
