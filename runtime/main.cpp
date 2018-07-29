#include "pl0.h"
#include <iostream>
using namespace std;

int main() {
	printf("-- begin --\n");

	if (pl0::load("../examples/test01.txt"))  return 1;
	// pl0::tokshow();
	if (pl0::parse())  return 1;
	pl0::progshow();
	if (pl0::run())  return 1;

	printf("-- end --\n");
}