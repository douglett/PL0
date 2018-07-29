#pragma once
#include "helpers.hpp"
// #include "tokenizer.hpp"

namespace pl0 {
using namespace std;

// stack frame definition
// struct frame {
// 	int return_address;  // code caller line number
// 	int dynamic_link;    // direct parent frame
// 	int static_link;     // link data segments in access order
// 	vector<int> data;    // raw data on stack
// };

// // init 2 data stores
// // vector<string> prog;
// vector<frame> stack = {{ 0, 0, 0, {} }};
// int PC = 0;

// int run() {
// 	printf("---\n");
// 	printf("running...\n");
// 	while (PC < (int)prog.size()) {
// 		const auto& cmd = prog[PC];
// 		printf("[%s]\n", join(cmd).c_str());
// 		// handle commands
// 		if      (cmd[0] == "CON")  { stack.back().data.push_back(stoi(cmd[2], NULL, 10)); }
// 		else if (cmd[0] == "VAR")  { stack.back().data.push_back(0); }
// 		else if (cmd[0] == "proc") { /* noop */ }
// 		else if (cmd[0] == "LIT")  { stack.back().data.push_back(stoi(cmd[1], NULL, 10)); }
// 		else {
// 			// throw string("unknown command: "+cmd[0]);
// 			fprintf(stderr, "error: unknown command: %s\n", cmd[0].c_str());
// 			return 1;
// 		}
// 		PC++;
// 	}
// 	return 0;
// }

static vector<int> stack = { 0, 0, 0 };  // initial state
static int PC = 0;
static int return_address = 0, dynamic_link = 0, static_link = 0;

int run() {
	while (PC < (int)prog.size()) {
		const auto& op = prog[PC];
		if       (op.type == "INT"){  stack.resize( stack.size() + op.b, 0 ), PC++;  }
		else if  (op.type == "JMP"){  PC = op.b;  }
		else if  (op.type == "LIT"){  stack.push_back(op.b), PC++;  }
		// else if  (op.type == "STO"){  }
		else     return fprintf(stderr, "error: unknown opcode: [%s]\n", op.type.c_str()), 1;
	}

	printf("run OK\n");
	return 0;
}

}