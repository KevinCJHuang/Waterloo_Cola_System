#pragma once


_Monitor Printer {
	public:
		enum Kind { Parent, Groupoff, WATCardOffice, NameServer, Truck,
			BottlingPlant, Student, Vending, Courier };
	private:
		const unsigned int numStudents, numVendingMachines, numCouriers;
		struct PState {
			Kind kind;
			unsigned int lid = -1;
			char state = '0';
			unsigned int value1 = -1;
			unsigned int value2 = -1;
			bool isActive = true;
			// ctors
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
		PState* buffer;
		void insertState(PState pState);
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
