#include "groupOff.h"
#include "printer.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
	:numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), printer(prt) {
		giftCards = new WATCard::FWATCard [numStudents];
	}

void Groupoff::main() {
	unsigned int i;
	unsigned int studentsDone = 0;
	for ( i = 0; i < numStudents; i++) {
		_Accept (giftCard);
		studentsDone++;
	}

	for ( i = 0; i < numStudents; i++) {
		giftCards[i].delivery(new WATCard());
	_Accept (~Groupoff) { break; } _Else
	}
}

WATCard::FWATCard Groupoff::giftCard() {
	return giftCards[studentsDone];
}
