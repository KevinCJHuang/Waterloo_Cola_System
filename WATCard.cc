#include "WATCard.h"

WATCard::WATCard() {
  FWATCard fCard;
}
void deposit( unsigned int amount ) {
  balance += amount;
}
void withdraw( unsigned int amount ) {
  balance -= amount;
}
unsigned int getBalance() { return balance; }
