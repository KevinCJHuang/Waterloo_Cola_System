#pragma once
#include "WATCard.h"

_Monitor Printer;
_Task Groupoff {
	void main();
  public:
	Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
	WATCard::FWATCard giftCard();
};
