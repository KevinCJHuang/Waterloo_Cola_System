#include "parent.h"
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;
void Parent::main() {
  printer.print(Printer::Kind::Parent, 'S');
  for ( ;; ) {
    _Accept (~Parent) { break; } // dtor => terminate
    _Else {
      unsigned amount = mprng(1,3);                 // choose random amount
      unsigned studentId = mprng(numStudents - 1);  // choose random student id
      yield(parentalDelay);                         // Yield before each transfer
      bank.deposit(studentId, amount);              // deposit money to bank
      printer.print(Printer::Kind::Parent, 'D', studentId, amount);
    }
  }
  printer.print(Printer::Kind::Parent, 'F');
}
