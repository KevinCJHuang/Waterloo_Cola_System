#include "groupOff.h"
#include "printer.h"
#include "WATCard.h"
#include "MPRNG.h"
extern MPRNG mprng;
Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
	:numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), printer(prt) {
	giftCards = new WATCard::FWATCard [numStudents];
}

void Groupoff::main() {
	printer.print(Printer::Kind::Groupoff, 'S');
	unsigned int i;
	for ( i = 0; i < numStudents; i++) _Accept (giftCard);
	
	for ( i = numStudents - 1; ; i--) {
		_Accept (~Groupoff) { break; }
		_Else {
			yield(groupoffDelay);
			unsigned int pos = mprng(i);
			WATCard* newCard = new WATCard;
			newCard->deposit(sodaCost);
			printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
			swap (giftCards[pos], giftCards[i]);
			giftCards[i].delivery(newCard);
		if (i == 0) break;
		}
	}

	printer.print(Printer::Kind::Groupoff, 'F');
}

WATCard::FWATCard Groupoff::giftCard() {
	return giftCards[studentsDone++];
}

Groupoff::~Groupoff() {
	delete [] giftCards;
}


