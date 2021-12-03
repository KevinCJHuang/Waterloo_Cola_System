#pragma once

_Monitor Printer;
_Task VendingMachine;
_Task NameServer {
		unsigned int numVendingMachines, numStudents;
		Printer& printer;
		void main();

		// added fields
		VendingMachine** machineList;	// vm positions
		unsigned int* studVMs; // array of index of the next vm to visit for each student 
		unsigned int curId;		 // the id of the current student or vm; for print msg
		unsigned int registeredMachines = 0;	// # of registered vm

  public:
		NameServer( Printer & prt, unsigned int numVendingMachines, unsigned int numStudents );
		~NameServer();
		void VMregister( VendingMachine * vendingmachine );
		VendingMachine * getMachine( unsigned int id );
		VendingMachine ** getMachineList();
};
