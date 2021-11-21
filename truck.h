#pragma once
_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;
_Task VendingMachine;
_Task Truck {
	Printer & printer;
	NameServer & nameServer;
	BottlingPlant & plant;
	const unsigned int numVendingMachines, maxStockPerFlavour, numFlavours = 4;
	VendingMachine** machineList;
	int curvm = 0;

	void main();
  public:
	Truck( Printer & prt, NameServer & nameServer, BottlingPlant & plant,
		   unsigned int numVendingMachines, unsigned int maxStockPerFlavour )
			 : printer(prt), nameServer(nameServer), plant(plant),
			 numVendingMachines(numVendingMachines), maxStockPerFlavour(maxStockPerFlavour) {};
};
