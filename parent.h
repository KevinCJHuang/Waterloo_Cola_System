#pragma once

_Monitor Bank;
_Monitor Printer;
_Task Parent {
	Printer& printer;
	Bank& bank;
	unsigned int numStudents, parentalDelay;
	void main();
  public:
	Parent( Printer & prt, Bank & bank, unsigned int numStudents, unsigned int parentalDelay )
		: printer(prt), bank(bank), numStudents(numStudents), parentalDelay(parentalDelay) {};
};
