#pragma once
#include "WATCard.h"

_Monitor Printer;
_Task Groupoff {
	unsigned int numStudents, sodaCost, groupoffDelay, studentsDone = 0;
	Printer & printer;
	WATCard::FWATCard* giftCards;	// array of gift cards for students
	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	~Groupoff();
	WATCard::FWATCard giftCard();
};
