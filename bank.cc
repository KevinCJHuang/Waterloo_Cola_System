#include "bank.h"
Bank::Bank( unsigned int numStudents ) : numStudents(numStudents) {
  studentAccounts = new unsigned int [numStudents] {0};
}

Bank::~Bank() {
  delete [] studentAccounts;
}

// Called by Parent
void Bank::deposit( unsigned int id, unsigned int amount ) {
  studentAccounts[id] += amount;
}

// Called by WATOffice::Courier
void Bank::withdraw( unsigned int id, unsigned int amount ) {
  while (studentAccounts[id] < amount) _Accept (deposit);
  studentAccounts[id] -= amount;
}

