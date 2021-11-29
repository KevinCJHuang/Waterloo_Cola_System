#include "parent.h"
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

extern MPRNG mprng;
void Parent::main() {
  printer.print(Printer::Kind::Parent, 'S');
  for ( ;; ) {
    _Accept (~Parent) { break; } _Else {}
    unsigned amount = mprng(1,3);
    unsigned studentId = mprng (0, numStudents - 1);
    cout << endl << "parent 2 mprngs done" << endl;
    yield(parentalDelay); // debug: checked
    bank.deposit(studentId, amount);
    printer.print(Printer::Kind::Parent, 'D', studentId, amount);
  }
  printer.print(Printer::Kind::Parent, 'F');
}
