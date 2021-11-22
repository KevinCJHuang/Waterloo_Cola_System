#pragma once
#include "WATCard.h"

_Monitor Printer;
_Task Groupoff {
	unsigned int numStudents, sodaCost, groupoffDelay;
	Printer & printer;
	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
		:numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), printer(prt) {}
	WATCard::FWATCard giftCard();
};
