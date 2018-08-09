#pragma once
#include "helpers.hpp"

namespace pl0 {
using namespace std;

class VM {
private:
	vector<Symbol> prog;
	vector<int> stack = { 0, 0, 0 };  // initial state
	int PC = 0, base = 0;
	// static int dynamic_link = 0, return_address = 0, static_link = 0;
public:
	istream* rin  = &cin;
	ostream* rout = &cout;

private:
	int framepos(int level) {
		int mbase = base;
		while (level > 0)
			mbase = stack[mbase + 2], level--;
		return mbase;
	}
	// static int& framevar(int level, int off) {
	// 	return stack[framepos(level) + off];
	// }
	void framepush() {
		stack.insert(stack.end(), { base, PC, 0 });
		base = stack.size() - 3;  // save new stack frame base
	}
	void framepop() {
		int obase = base;  // original frame base position
		PC   = stack[base + 1];
		base = stack[base + 0];
		stack.resize(obase);
	}
	void showstate() {
		string s;
		for (int i=0; i<(int)stack.size(); i++)
			s += (i>0 ? "," : "") + to_string(stack[i]);
		printf(":%-4d  %s  ", PC, s.c_str());
		// getline(cin, s);
		printf("\n");
	}

public:
	void reset() {
		stack = { 0, 0, 0 };  // initial state
		PC = base = 0;
	}
	int run(vector<Symbol>& prog) {
		this->prog = prog;
		reset();
		return run();
	}
	int run() {
		try {
		while (PC < (int)prog.size()) {
			// debug values
			// showstate();
			// do operation
			const auto& op = prog[PC];
			if       (op.type == "INT"){  stack.resize( stack.size() + op.b, 0 ), PC++;  }
			else if  (op.type == "JMP"){  PC = op.b;  }
			else if  (op.type == "JPC"){  stack.back() == 0 ? PC = op.b : PC++;  }
			else if  (op.type == "LIT"){  stack.push_back(op.b), PC++;  }
			else if  (op.type == "STO"){  stack[framepos(op.a) + op.b] = stack.back(); stack.pop_back(), PC++;  }
			else if  (op.type == "LOD"){  stack.push_back( stack[framepos(op.a) + op.b] ), PC++;  }
			else if  (op.type == "CAL"){  framepush(), PC = op.b;  }
			else if  (op.type == "RET"){  framepop() , PC++;  }
			else if  (op.type == "OPR"){  
				int i = stack.back(); stack.pop_back();  // pop item into register
				switch (op.b) {
					case 1:    stack.back() += i; break;
					case 2:    stack.back() -= i; break;
					case 3:    stack.back() *= i; break;
					case 4:    stack.back() /= i; break;
					case 5:    stack.back() = (stack.back() == i); break;
					case 6:    stack.back() = (stack.back() != i); break;
					case 7:    stack.back() = (stack.back() <  i); break;
					case 8:    stack.back() = (stack.back() >  i); break;
					case 9:    stack.back() = (stack.back() <= i); break;
					case 10:   stack.back() = (stack.back() >= i); break;
					case 11:   stack.push_back(i % 2); break;
					default:  throw string("unknown OPR opcode: ["+to_string(op.b)+"]");
				}
				PC++;  }
			else if  (op.type == "EXT"){  
				if      (op.b == 1){  *rout << "> " << stack.back() << endl; stack.pop_back();  }
				else if (op.b == 2){  int i; *rout << ">> "; *rin >> i; stack.push_back(i);  }
				else    throw string("unknown EXT id: "+to_string(op.b));
				PC++;  }
			else     throw string("unknown opcode: ["+op.type+"]");
		}
		}
		catch (const string& err) {
			fprintf(stderr, 
				"runtime error: %s\n"
				"	line %d\n",
				err.c_str(), PC );
			return 1;
		}
		// OK
		printf("run OK\n");
		return 0;
	}
}; // end class

} // end namespace