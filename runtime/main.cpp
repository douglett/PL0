#include "pl0.h"
#include <iostream>
using namespace std;

int testfile(const string& fname, const string& input="") {
	printf("-- testing [%s] --\n", fname.c_str());
	// tokenize
	pl0::Tokenizer tok;
	if (tok.load("../examples/" + fname))  return 1;
	// tok.tokshow();
	// parse
	pl0::Parser parser;  // parser and emitter
	if (parser.parse(tok.toklist))  return 1;
	// parser.emitter.progshow();
	// vm
	pl0::VM vm;
	stringstream ss(input);
	if (input != "")  vm.rin = &ss;
	if (vm.run(parser.emitter.prog))  return 1;
	// OK
	return 0;
}

int testall() {
	static const vector<string> fnames = {
		"test00.txt", "test01.txt", "test02-1.txt", "test02-2.txt",
		"test03.txt", "test04.txt", "test05.txt", "test06.txt", "test09.txt" };
	for (auto f : fnames)
		if (testfile(f, "1 2 3 4"))  return 1;
	return 0;
}

int main() {
	printf("-- begin --\n");
	// if (testall())  return 1;
	testfile("test09.txt");
	printf("-- end --\n");
}