#pragma once
_Monitor Printer;
_Task VendingMachine;
_Task NameServer {
		unsigned int numVendingMachines, numStudents, curId;
		VendingMachine** machineList;
		Printer& printer;
		unsigned int* studVMs;
		void main();

  public:
		NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
		// ~NameServer();
		void VMregister( VendingMachine * vendingmachine );
		VendingMachine * getMachine( unsigned int id );
		VendingMachine ** getMachineList();
};
