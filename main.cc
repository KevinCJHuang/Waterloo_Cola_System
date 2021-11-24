#include <iostream>
#include <string>
#include <unistd.h>										// access: getpid

// Kevin: main.cc may not need to include all these; only included to test compile for now
#include "MPRNG.h"
#include "bank.h"
#include "bottlingPlant.h"
#include "config.h"
#include "groupOff.h"
#include "nameServer.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "vendingMachine.h"
#include "WATCard.h"
#include "WATCardOffice.h"

using namespace std;
MPRNG mprng;
int main( int argc, char * argv[] ) {
  // default values
  ConfigParms configParms;
  string configFile = "soda.config";
  unsigned int processors=1, seed=getpid();

	try { // Read command line inputs
		switch ( argc ) {
      case 4: if ( strcmp( argv[3], "d" ) != 0 ) {	// default ?
				processors = stoi( argv[3] ); if ( processors <= 0 ) throw 1;
			} // if
		  case 3: if ( strcmp( argv[2], "d" ) != 0 ) {	// default ?
				seed = stoi( argv[2] ); if ( seed <= 0 ) throw 1;
			} // if
		  case 2: if ( strcmp( argv[1], "d" ) != 0 ) {	// default ?
				configFile = argv[1];
			} // if
		  case 1: break;								// use all defaults
		  default: throw 1;
		} // switch
	} catch( ... ) {
		cerr << "Usage: " << argv[0] << " [ config-file | 'd' (default file "
    << "soda.config) [ seed (> 0) | 'd' (default random) [ processors (> 0) | "
    << "'d' (default 1) ] ] ]" << endl;
		exit( EXIT_FAILURE );
	} // try
  
	mprng.set_seed(seed);
  // Init processors
  uProcessor p[processors - 1]; // number of kernel threads
  if ( processors == 1 ) uThisProcessor().setPreemption( 0 );

  processConfigFile(configFile.c_str(), configParms); // Read configs

  // Create everything
	Printer printer(configParms.NumStudents, configParms.NumVendingMachines, configParms.NumCouriers);
	Bank bank(configParms.NumStudents);
	Parent parent(printer, bank, configParms.NumStudents, configParms.ParentalDelay);
	WATCardOffice wOffice(printer, bank, configParms.NumCouriers);
	Groupoff groupOff(printer, configParms.NumStudents, configParms.SodaCost, configParms.GroupoffDelay);
	NameServer nameServer(printer, configParms.NumVendingMachines, configParms.NumStudents);
	VendingMachine** vendingMachines = new VendingMachine[configParms.NumVendingMachines];
	for (int i = 0; i < configParms.NumVendingMachines; i++) {
		vendingMachines[i] = new VendingMachine(printer, nameServer, i, configParms.SodaCost);
	}
	BottlingPlant plant = new bottlingPlant(printer, nameServer,
			configParms.NumVendingMachines, configParms.MaxShippedPerFlavour,
			configParms.MaxStockPerFlavour, configParms.TimeBetweenShipments);
	Students** students = new Student[configParms.NumStudents];
	for (int i = 0; i < configParms.NumStudents; i++) {
		students[i] = new Student(printer, nameServer, wOffice, groupOff, i);
	}

	// wait till all students finish and delete them
	for (int i = 0; i < configParms.NumStudents; i++) {
		delete students[i];
	}
	delete students;

	delete plant;
	for (int i = 0; i < configParms.NumVendingMachines; i++) {
		delete vendingMachines[i];
	}
	delete vendingMachines;

} // main
