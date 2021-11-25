#pragma once

_Monitor Bank {
		unsigned int * studentAccounts; // Records student account balances
		uCondition * bench;				// Array of uCondition; one bench for each student/courier
		unsigned int numStudents;
  public:
		Bank( unsigned int numStudents );
		~Bank();
		void deposit( unsigned int id, unsigned int amount );
		void withdraw( unsigned int id, unsigned int amount );
};
