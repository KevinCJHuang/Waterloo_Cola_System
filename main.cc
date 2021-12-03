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
  
  // Init processors
  uProcessor p[processors - 1]; // number of kernel threads
  if ( processors == 1 ) uThisProcessor().setPreemption( 0 );

  processConfigFile(configFile.c_str(), configParms); // read configs from file
	mprng.set_seed(seed);	// set mprng seed

  // Create everything
	Printer printer(configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers);
	Bank bank(configParms.numStudents);
	Parent parent(printer, bank, configParms.numStudents, configParms.parentalDelay);
	WATCardOffice wOffice(printer, bank, configParms.numCouriers);
	Groupoff groupOff(printer, configParms.numStudents, configParms.sodaCost, configParms.groupoffDelay);
	NameServer nameServer(printer, configParms.numVendingMachines, configParms.numStudents);
	VendingMachine* vendingMachines [configParms.numVendingMachines];

	// create vm
	for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
		vendingMachines[i] = new VendingMachine(printer, nameServer, i, configParms.sodaCost);
	} // for
	
	{	// delete plant before vending machines; plant will be deleted by the end of this block
		BottlingPlant plant(printer, nameServer,
			configParms.numVendingMachines, configParms.maxShippedPerFlavour,
			configParms.maxStockPerFlavour, configParms.timeBetweenShipments);
		
		// create students
		Student* students[configParms.numStudents];
		for (unsigned int i = 0; i < configParms.numStudents; i++) {
			students[i] = new Student(printer, nameServer, wOffice, groupOff, i, configParms.maxPurchases);
		} // for

		// wait till all students finish and delete them
		for (unsigned int i = 0; i < configParms.numStudents; i++) {
			delete students[i];
		} // for
	} // by now, the plant is terminated
	
	// wait till all vm finish and delete them
	for (unsigned int i = 0; i < configParms.numVendingMachines; i++) {
		delete vendingMachines[i];
	} // for
} // main

