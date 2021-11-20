#include <iostream>
#include <string>
#include <unistd.h>										// access: getpid

// Kevin: main.cc may not need to include all these; only included to test compile for now
#include "bank.h"
#include "bottlingPlant.h"
#include "config.h"
#include "groupOff.h"
#include "nameServer.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "truck.h"
#include "vendingMachine.h"
#include "WATCard.h"
#include "WATCardOffice.h"

using namespace std;

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

  processConfigFile(configFile.c_str(), configParms); // Read configs

	Printer printer(configParms.numStudents, configParms.numVendingMachines, configParms.numCouriers);
	printer.print(Printer::Kind::Truck, 'S');
	printer.print(Printer::Kind::Truck, 'P', 1);
	printer.print(Printer::Kind::Truck, 'd', 1, 2);
	printer.print(Printer::Kind::Student, 1, 'r');
	printer.print(Printer::Kind::Student, 1, 'V', 1);
	printer.print(Printer::Kind::Student, 1, 'B', 1, 2);
	printer.print(Printer::Kind::Student, 1, 'F');

	// printer.print(Printer::Kind::Parent, 'S');
	// printer.print(Printer::Kind::BottlingPlant, 'S');
	// printer.print(Printer::Kind::Truck, 'P', 6);
	// printer.print(Printer::Kind::Student, 1, 'S');
	// printer.print(Printer::Kind::Student, 0, 'F');

	// printer.print(Printer::Kind::Truck, 'd', 0, 6);
	// printer.print(Printer::Kind::Truck, 'S');
	// printer.print(Printer::Kind::Truck, 'D', 0, 14);

} // main
