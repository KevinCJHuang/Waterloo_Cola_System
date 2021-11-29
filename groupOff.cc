#include "groupOff.h"
#include "printer.h"
#include "WATCard.h"
#include "MPRNG.h"
extern MPRNG mprng;
Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
	:numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), printer(prt) {
	giftCards = new WATCard::FWATCard[numStudents];
}

void Groupoff::main() {
	printer.print(Printer::Kind::Groupoff, 'S');
	unsigned int i;
	unsigned int studentsDone = 0;
	for ( i = 0; i < numStudents; i++) {
		_Accept (giftCard);
		studentsDone++;
	}

	bool created [numStudents] {false};
	unsigned int createdGiftCard = 0;
	for ( ;; ) {
		_Accept (~Groupoff) { break; } _Else {}

		unsigned int pos = mprng(0, numStudents-1);
		cout << endl<<"groupoff randomed!" << endl;
		if (!created[pos]) {
			yield(groupoffDelay);
			WATCard* newCard = new WATCard;
			newCard->deposit(sodaCost);
			giftCards[pos].delivery(newCard);
			printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
			created[pos] = true;
			if (++createdGiftCard == numStudents) break;
		}
	}

	printer.print(Printer::Kind::Groupoff, 'F');
}

WATCard::FWATCard Groupoff::giftCard() {
	return giftCards[studentsDone];
}

Groupoff::~Groupoff() {
	delete [] giftCards;
}