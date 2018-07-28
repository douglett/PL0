#include "pl0.h"
#include <iostream>
using namespace std;

int main() {
	cout << "here" << endl;

	pl0::load("examples/test.txt");
	// pl0::tokshow();
	pl0::parse();
	// pl0::run();
}