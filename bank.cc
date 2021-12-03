#include "bank.h"
Bank::Bank( unsigned int numStudents ) {
  studentAccounts = new unsigned int [numStudents] {0};
}

Bank::~Bank() {
  delete [] studentAccounts;
}

// Called by Parent
void Bank::deposit( unsigned int id, unsigned int amount ) {
  studentAccounts[id] += amount;  // increase amount to student account
}

// Called by WATOffice::Courier
void Bank::withdraw( unsigned int id, unsigned int amount ) {
  for ( ;; ) {
  if (studentAccounts[id] >= amount) break;
    _Accept (deposit); // Not enough money; wait for more deposits
  }
  studentAccounts[id] -= amount;  // decrease amount to student account
}
