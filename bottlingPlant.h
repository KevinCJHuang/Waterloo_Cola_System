#pragma once
#include "truck.h"

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant {
		void main();
		unsigned int numVendingMachines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments; 
		Printer& printer;
		NameServer& nameServer;
		unsigned int stock [4] = {0};	// stocks generated in the current production run
		bool isShutdown = false;			// flag variable to shut down the plant & truck
  public:
		_Event Shutdown {};					// shutdown plant
		BottlingPlant( Printer & prt, NameServer & nameServer,
			unsigned int numVendingMachines, unsigned int maxShippedPerFlavour,
			unsigned int maxStockPerFlavour, unsigned int timeBetweenShipments );
		void getShipment( unsigned int cargo[] );
};
