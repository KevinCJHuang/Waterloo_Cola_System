#include "groupOff.h"
#include "printer.h"

Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
	:numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), printer(prt) { }

void Groupoff::main() {
	printer.print(Printer::Kind::Groupoff, 'S');
	unsigned int i;
	unsigned int studentsDone = 0;
	for ( i = 0; i < numStudents; i++) {
		_Accept (giftCard);
		studentsDone++;
	}

	for ( i = 0; i < numStudents; i++) {
		yield(groupoffDelay);
		WATCard* newCard = new WATCard;
		newCard.deposit(sodaCost);
		giftCards[i].delivery(newCard);
		printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
		_Accept (~Groupoff) { break; } _Else
	}
	printer.print(Printer::Kind::Groupoff, 'F');
}

WATCard::FWATCard Groupoff::giftCard() {
	return giftCards[studentsDone];
}
