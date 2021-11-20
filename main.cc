#include <iostream>
#include <string>
#include <unistd.h>										// access: getpid
#include "config.h"

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

} // main
