#include "pl0.h"
#include "tokenizer.hpp"
#include <iostream>
using namespace std;

int main() {
	cout << "here" << endl;

	pl0::load("examples/test.txt");
	printf("token count: %d\n", pl0::toklist.size());
	for (const auto& t : pl0::toklist)
		printf("	%03d [%s] %s\n", t.line, t.val.c_str(), t.type.c_str());
}