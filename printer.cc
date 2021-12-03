#include "printer.h"

// Printer's constructor
Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines,
	unsigned int numCouriers ) : numStudents(numStudents),
	numVendingMachines(numVendingMachines), numCouriers(numCouriers) {
	// Print first line
	cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant\t";
	for (unsigned int i = 0; i < numStudents; i++) cout << "Stud" << i << "\t";
	for (unsigned int i = 0; i < numVendingMachines; i++) cout << "Mach" << i << "\t";
	for (unsigned int i = 0; i < numCouriers-1; i++) cout << "Cour" << i << "\t";
	cout << "Cour" << numCouriers-1 << endl;

	// Print second line
	for (unsigned int i = 0; i < 5 + numStudents + numVendingMachines + numCouriers; i++) {
		cout << "*******\t";
	}
	cout << "*******" << endl;

	// Initialize buffer
	buffer = new PState[6 + numStudents + numVendingMachines + numCouriers];
};

// Printer's dtor
Printer::~Printer() {
	// flush and print whatever's left in the buffer
  for (unsigned int i = 0; i < 6 + numStudents + numVendingMachines; i++) {
    if (buffer[i].isActive) {
			// if current state is not printed yet, insert an arbitrary state into
			// the buffer to flush the last line of PStates
			Kind kind = buffer[i].kind;
			int lid = buffer[i].lid;
			lid == -1 ? insertState(PState(kind, 'F'))
				: insertState(PState(kind, lid, 'F'));
      break;
    }
  }
  cout << "***********************" << endl;
  delete []buffer;
}

// Printer::print inserts a corresponding PState into the buffer.
// Printer::insertState will manage when to flush the buffer and print
void Printer::print( Kind kind, char state ) {
	insertState(PState(kind, state));
}
void Printer::print( Kind kind, char state, unsigned int value1 ) {
	insertState(PState(kind, state, value1));
}
void Printer::print( Kind kind, char state, unsigned int value1, unsigned int value2 ) {
	insertState(PState(kind, state, value1, value2));
}
void Printer::print( Kind kind, unsigned int lid, char state ) {
	insertState(PState(kind, lid, state));
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1) {
	insertState(PState(kind, lid, state, value1));
}
void Printer::print( Kind kind, unsigned int lid, char state, unsigned int value1,
	unsigned int value2 ) {
	insertState(PState(kind, lid, state, value1, value2));
}

// Insert into the buffer, and print accordingly
void Printer::insertState(PState pState) {
  unsigned int tabCounter = 0; 	  // Used to track how many \t to print
	unsigned int pos = pState.kind; // position/index of the state in buffer
	switch (pState.kind) { 					// Special index for student, vm, courier
		case Courier:
			pos += numVendingMachines;
		case Vending:
			pos += numStudents;
		case Student:
			pos += 6 + pState.lid - pState.kind;
			break;
	} // switch

	// isActive ? flush and print
	if (buffer[pos].isActive) {
		// traverse through the buffer, and print active states
		for (unsigned int i = 0; i < 6 + numStudents + numCouriers + numVendingMachines; i++) {
			if (i) tabCounter++;
		if (!buffer[i].isActive) continue; // not active => don't print
      
			// Print all \t, and reset tabCounter
      for ( unsigned int i = 0; i < tabCounter; i++) cout << "\t"; 
      tabCounter = 0;

			// print current state (buffer[pos])
      PState& state = buffer[i];
      cout << state.state;
      switch (state.kind) {
				case Parent:
					if (state.state == 'D') cout << state.value1 << "," << state.value2;
					break;
				case Groupoff:
					if (state.state == 'D') cout << state.value1;
					break;
				case WATCardOffice:
					switch (state.state) {
						case 'C':
						case 'T':
							cout << state.value1 << "," << state.value2;
					}
					break;
				case NameServer:
					switch (state.state) {
						case 'R':
							cout << state.value1;
							break;
						case 'N':
							cout << state.value1 << "," << state.value2;
					}
					break;
				case Truck:
					switch (state.state) {
						case 'P':
							cout << state.value1;
							break;
						case 'd':						
						case 'D':
						case 'U':
							cout << state.value1 << "," << state.value2;
					}
					break;
				case BottlingPlant:
					if (state.state == 'G') cout << state.value1;
					break;
				case Student:
					switch (state.state) {
						case 'V':
							cout << state.value1;
							break;
						case 'S':						
						case 'G':
						case 'a':
						case 'A':
						case 'B':
							cout << state.value1 << "," << state.value2;					
					}
					break;
				case Vending:
					switch (state.state) {
						case 'S':
							cout << state.value1;
							break;
						case 'B':
							cout << state.value1 << "," << state.value2;					
					}
					break;
				case Courier:
					switch (state.state) {
						case 'L':
							cout << state.value1;
							break;
						case 't':
						case 'T':
							cout << state.value1 << "," << state.value2;					
					}
      } // switch
			buffer[i].isActive = false; // deactivate the current state
		} // for
		cout << endl;
	}
	buffer[pos] = pState; // Store new state
} // insertState
