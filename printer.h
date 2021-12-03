#pragma once
#include <iostream>
using namespace std;

_Monitor Printer {
	public:
		enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck,
			BottlingPlant, Student, Vending, Courier };
	private:
		const unsigned int numStudents, numVendingMachines, numCouriers;

		// each PState represents a state to be printed in the printer buffer 
		struct PState {
			Kind kind;									// Printer::Kind
			unsigned int lid = -1;			// lid to be printed, if there is any
			char state = '0';						// state to be printed
			unsigned int value1 = -1; 	// lid to be printed, if there is any
			unsigned int value2 = -1; 	// lid to be printed, if there is any
			bool isActive = true;				// if isActive, then the state is not printed yet

			// ctors, used by Printer::print when inserting a PState into the buffer with insertState
			PState(): isActive (false) {};
			PState( Kind kind, char state ): kind(kind), state(state) {};
			PState( Kind kind, char state, unsigned int value1 ): kind(kind), state(state), value1(value1) {};
			PState( Kind kind, char state, unsigned int value1, unsigned int value2 )
				: kind(kind), state(state), value1(value1), value2(value2) {};
			PState( Kind kind, unsigned int lid, char state )
				: kind(kind), lid(lid), state(state) {};
			PState( Kind kind, unsigned int lid, char state, unsigned int value1 )
				: kind(kind), lid(lid), state(state), value1(value1) {};
			PState( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 )
				: kind(kind), lid(lid), state(state), value1(value1), value2(value2) {};
		};
		PState* buffer; 								 // buffer of all PStates
		void insertState(PState pState); //inserts a PState into the buffer; flush & print
		
  public:
		Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );		
		~Printer();
		void print( Kind kind, char state );
		void print( Kind kind, char state, unsigned int value1 );
		void print( Kind kind, char state, unsigned int value1, unsigned int value2 );
		void print( Kind kind, unsigned int lid, char state );
		void print( Kind kind, unsigned int lid, char state, unsigned int value1 );
		void print( Kind kind, unsigned int lid, char state, unsigned int value1, unsigned int value2 );
};
