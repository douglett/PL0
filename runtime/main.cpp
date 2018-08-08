#include "pl0.h"
#include <iostream>
using namespace std;

int main() {
	printf("-- begin --\n");

	// tokenize
	pl0::Tokenizer tok;
	if (tok.load("../examples/test03.txt"))  return 1;
	// tok.tokshow();
	// parse
	pl0::Parser parser;  // parser and emitter
	if (parser.parse(tok.toklist))  return 1;
	parser.emitter.progshow();
	// vm
	pl0::VM vm;
	if (vm.run(parser.emitter.prog))  return 1;

	printf("-- end --\n");
}