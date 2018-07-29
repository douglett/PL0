#pragma once
#include "helpers.hpp"

namespace pl0 {
using namespace std;

static vector<int> stack = { 0, 0, 0 };  // initial state
static int PC = 0, base = 0;
// static int dynamic_link = 0, return_address = 0, static_link = 0;

static int framepos(int level) {
	int mbase = base;
	while (level > 0)
		mbase = stack[mbase + 2], level--;
	return mbase;
}
// static int& framevar(int level, int off) {
// 	return stack[framepos(level) + off];
// }
static void framepush() {
	stack.insert(stack.end(), { base, PC, 0 });
	base = stack.size() - 3;  // save new stack frame base
}
static void framepop() {
	int obase = base;  // original frame base position
	PC   = stack[base + 1];
	base = stack[base + 0];
	stack.resize(obase);
}
static void showstate() {
	printf(":%-4d  ", PC);
	for (int i=0; i<(int)stack.size(); i++)
		printf("%s%d", (i>0 ? "," : ""), stack[i]);
	printf("   \n");
	// string s;  getline(cin, s);
}

int run() {
	try {
	while (PC < (int)prog.size()) {
		// showstate();
		const auto& op = prog[PC];
		if       (op.type == "INT"){  stack.resize( stack.size() + op.b, 0 ), PC++;  }
		else if  (op.type == "JMP"){  PC = op.b;  }
		else if  (op.type == "LIT"){  stack.push_back(op.b), PC++;  }
		else if  (op.type == "STO"){  stack[framepos(op.a) + op.b] = stack.back(); stack.pop_back(), PC++;  }
		else if  (op.type == "LOD"){  stack.push_back( stack[framepos(op.a) + op.b] ), PC++;  }
		else if  (op.type == "CAL"){  framepush(), PC = op.b;  }
		else if  (op.type == "RET"){  framepop() , PC++;  }
		else if  (op.type == "EXT"){  
			if      (op.b == 1)  printf("> %d\n", stack.back()), stack.pop_back();
			else if (op.b == 2)  throw string("err!");
			else    throw string("unknown EXT id: "+to_string(op.b));
			PC++;  }
		else     throw string("unknown opcode: ["+op.type+"]");
	}
	}
	catch (const string& err) {
		fprintf(stderr, "runtime error: %s\n", err.c_str());
		fprintf(stderr, "	line %d\n", PC);
		return 1;
	}
	// OK
	printf("run OK\n");
	return 0;
}

}