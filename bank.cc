#include "bank.h"
Bank::Bank( unsigned int numStudents ) : numStudents(numStudents) {
  studentAccounts = new unsigned int [numStudents];
  for (unsigned int i = 0; i < numStudents; i++)
    studentAccounts[i] = 0;
  bench = new uCondition [numStudents];
}

// Called by Parent
void Bank::deposit( unsigned int id, unsigned int amount ) {
  studentAccounts[id] += amount;
  // Wake up a waiting courier to check if the amount is enough
  if (!bench[id].empty()) bench[id].signal();
}

// Called by WATOffice::Courier
void Bank::withdraw( unsigned int id, unsigned int amount ) {
  while (studentAccounts[id] <= amount) bench[id].wait();
  studentAccounts[id] -= amount;
}

