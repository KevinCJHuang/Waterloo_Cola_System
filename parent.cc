#include "parent.h"
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;
void Parent::main() {
  printer.print(Printer::Kind::Parent, 'S');
  for ( ;; ) {
    unsigned amount = mprng(1,3);
    unsigned studentId = mprng (0, numStudents - 1);
    yield(parentalDelay);
    _Accept (~Parent) { break; } _Else
    bank.deposit(studentId, amount);
    printer.print(Printer::Kind::Parent, 'D', studentId, amount);
  }
  printer.print(Printer::Kind::Parent, 'F');
}
