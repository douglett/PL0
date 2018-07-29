#pragma once
#include "helpers.hpp"

namespace pl0 {
using namespace std;

// consts
static const vector<string> OPR_LIST = {
	"NIL",  "+","-","*","/",  "=","!=","<",">","<=",">=" };

// symbol table storage
struct symbol {
	string type, name;
	int a, b;
};
static vector<vector<symbol>> table;
// program storage
vector<symbol> prog; 

//-- table management
static const symbol find_symbol(const string& id) {
	for (int i = table.size()-1; i >= 0; i--)
		for (int j = table[i].size()-1; j >= 0; j--)
			if (table[i][j].name == id)  return table[i][j];
	throw string("missing symbol: ["+id+"]");
}
static const int find_symbol_level(const string& id) {
	for (int i = table.size()-1; i >= 0; i--)
		for (int j = table[i].size()-1; j >= 0; j--)
			if (table[i][j].name == id)  return table.size()-1 - i;
	throw string("missing symbol: ["+id+"]");
}

//-- emitter
void emit(const vector<string>& args) {
	// printf("%s\n", join(args).c_str());

	if      (args[0] == "_block")  
		table.push_back({});
	else if (args[0] == "_end"){
		table.pop_back();
		prog.push_back({ "RET", "end" });  }
	else if (args[0] == "_const")  
		table.back().push_back({ "const", args[1], stoi(args[2]) });
	else if (args[0] == "_varlist"){
		for (int i=1; i<(int)args.size(); i++) 
			table.back().push_back({ "var", args[i], 2+i });
		auto label = join( vector<string>(args.begin()+1, args.end()) );
		prog.push_back({ "INT", label, 0, (int)args.size()-1 });  }
	else if (args[0] == "_procedure")  
		table.back().push_back({ "procedure", args[1], (int)prog.size() });
	else if (args[0] == "LIT")  
		prog.push_back({ "LIT", args[1], 0, stoi(args[1]) });
	else if (args[0] == "STO" || args[0] == "LOD"){ 
		auto& sym = find_symbol(args[1]);
		if (sym.type != "var")  throw string("emit: expected var");
		prog.push_back({ args[0], sym.name, find_symbol_level(args[1]), sym.a });  }
	else if (args[0] == "CAL"){ 
		auto& sym = find_symbol(args[1]);
		if (sym.type != "procedure")  throw string("emit: expected procedure");
		prog.push_back({ "CAL", args[1]+"()", find_symbol_level(args[1]), sym.a });  }
	else if (args[0] == "JMP" || args[0] == "JPC")
		prog.push_back({ args[0], "", 0, stoi(args[1]) });
	else if (args[0] == "OPR"){ 
		int opcode = -1;
		for (int i = 0; i < (int)OPR_LIST.size(); i++)
			if (OPR_LIST[i] == args[1]){  opcode = i; break;  }
		if (opcode == -1)  throw string("emit: unknown OPR: ["+args[1]+"]");
		prog.push_back({ "OPR", args[1], 0, opcode });  }
	else if (args[0] == "EXT"){ 
		int cmd = 0;
		if      (args[1] == "write")  cmd = 1;
		else if (args[1] == "read" )  cmd = 2;
		prog.push_back({ "EXT", args[1], 0, cmd });  }
	else    throw string("emit: unknown command ["+args[0]+"]");
}

void progshow() {
	for (int i = 0; i < (int)prog.size(); i++)
		printf("%-4d %s %3d,%-4d %s\n", 
			i,
			prog[i].type.c_str(), prog[i].a, prog[i].b, 
			prog[i].name.c_str() );
}

}