#include "parent.h"
#include "printer.h"
#include "bank.h"
#include "MPRNG.h"
#ifdef DEBUG
    using namespace std;
#endif

extern MPRNG mprng;
void Parent::main() {
  printer.print(Printer::Kind::Parent, 'S');
  for ( ;; ) {
    _Accept (~Parent) { break; } _Else {}
    unsigned amount = mprng(1,3);
    unsigned studentId = mprng (0, numStudents - 1);
    yield(parentalDelay); // debug: checked
    printer.print(Printer::Kind::Parent, 'D', studentId, amount);
    bank.deposit(studentId, amount);
  }
  printer.print(Printer::Kind::Parent, 'F');
}
