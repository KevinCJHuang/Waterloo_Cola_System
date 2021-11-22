#include "WATCard.h"

WATCard::WATCard() {}
void WATCard::deposit( unsigned int amount ) {
  balance += amount;
}
void WATCard::withdraw( unsigned int amount ) {
  balance -= amount;
}
unsigned int WATCard::getBalance() { return balance; }
