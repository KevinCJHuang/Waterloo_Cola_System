#pragma once
#include "truck.h"
_Monitor Printer;
_Task NameServer;
_Task BottlingPlant {
		void main();
		unsigned int numVendingMachines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments; 
		Printer& printer;
		NameServer& nameServer;
		unsigned int stock [4] = {0};
		bool isShutdown = false;
  public:
		_Event Shutdown {};					// shutdown plant
		BottlingPlant( Printer & prt, NameServer & nameServer,
			unsigned int numVendingMachines, unsigned int maxShippedPerFlavour,
			unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments )
			: numVendingMachines(numVendingMachines), maxShippedPerFlavour(maxShippedPerFlavour),
			maxStockPerFlavour(maxStockPerFlavour), timeBetweenShipments(timeBetweenShipments),
			printer(prt), nameServer(nameServer) {};
		void getShipment( unsigned int cargo[] );
};
