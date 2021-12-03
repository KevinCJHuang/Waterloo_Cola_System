#include "groupOff.h"
#include "printer.h"
#include "WATCard.h"
#include "MPRNG.h"
extern MPRNG mprng;
Groupoff::Groupoff( Printer & prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay )
	:numStudents(numStudents), sodaCost(sodaCost), groupoffDelay(groupoffDelay), printer(prt) {
	giftCards = new WATCard::FWATCard [numStudents]; // Create all FWATCards
}

Groupoff::~Groupoff() {
	delete [] giftCards;
}

WATCard::FWATCard Groupoff::giftCard() {
	return giftCards[studentsDone++];	// return the next card that's not distributed yet
}

void Groupoff::main() {
	printer.print(Printer::Kind::Groupoff, 'S');

	// give each student a future gift card
	for ( unsigned int i = 0; i < numStudents; i++)
		_Accept (giftCard); 
	
	// create & deliver gift cards
	for ( unsigned int i = numStudents - 1; ; i--) {
		_Accept (~Groupoff) { // dtor => terminate
			break; 
		} _Else {
			// The first i cards are not delivered yet.
			yield(groupoffDelay);							// yield before assigning a real card
			unsigned int pos = mprng(i);			// choose a random pos in [0,i]
			WATCard* newCard = new WATCard;		// create a new gift card 
			newCard->deposit(sodaCost);			  // deposit money
			printer.print(Printer::Kind::Groupoff, 'D', sodaCost);
			swap (giftCards[pos], giftCards[i]);	// swap chosen random position the last undelivered card
			giftCards[i].delivery(newCard);				// deliver the chosen random position (after swap)
			// Now all delivered cards are in indicdes [i-1, numStudents-1]
		if (i == 0) break; // end of loop
		} // _Accept
	} // for

	printer.print(Printer::Kind::Groupoff, 'F');
}
