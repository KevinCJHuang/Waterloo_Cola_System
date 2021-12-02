#include <uFuture.h>


int main () {
    Future_ISM<int *> fint;
    int x;
    int* y = new int(5);
    fint.delivery(y);
    delete y;
    fint.reset();
    malloc_stats();

}