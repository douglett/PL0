#include "pl0.h"
#include <iostream>
using namespace std;

int main() {
	cout << "begin" << endl;

	if (pl0::load("../examples/test.txt"))  return 1;
	// pl0::tokshow();
	if (pl0::parse())  return 1;
	pl0::progshow();
}