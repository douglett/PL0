#pragma once
#include "tokenizer.hpp"
#include "emitter.hpp"

namespace pl0 {
using namespace std;

static int pos = 0;

//-- next token functions
static const Node EOF_NODE = { "EOF", "EOF", -1 };
static const Node& nexttok() {
	if (pos >= (int)toklist.size())  return EOF_NODE;
	return toklist[pos];
}
static const Node& lasttok() {
	if (pos <= 0)  return EOF_NODE;
	return toklist[pos-1];
}
static int expect(const string& type, const string& val) {
	if (nexttok().type == "EOF")  return 0;
	if (nexttok().type != type)  return 0;
	if (val.size() && nexttok().val != val)  return 0;  // val can be blank (ignore)
	pos++;
	return 1;
}
static int require(const string& type, const string& val) {
	if (nexttok().type == "EOF")  throw string("unexpected EOL");
	if (!expect(type, val))  throw string("unexpected token. expected ["+type+"] ["+val+"]");
	return 1;
}

//-- parser functions
static void parse_expression();

static void parse_factor() {
	if      (expect("identifier", "")) { emit({ "LOD", lasttok().val }); }
	else if (expect("number", "")) { emit({ "LIT", lasttok().val }); }
	else if (expect("operator", "(")) { parse_expression();  require("operator", ")"); }
	else 	{ throw string("unexpected token in factor"); }
}

static void parse_term() {
	parse_factor();
	while (expect("operator", "*") || expect("operator", "/")) {
		auto& op = lasttok();
		parse_factor();
		emit({ "OPR", op.val });
	}
}

static void parse_expression() {
	// add or sub
	if (expect("operator", "+") || expect("operator", "-")) {
		auto& op = lasttok();
		parse_term();
		emit({ "OPR", op.val });
	}
	// none - just parse term
	else {
		parse_term();
	}
	// if add or sub, loop
	while (expect("operator", "+") || expect("operator", "-")) {
		auto& op = lasttok();
		parse_term();
		emit({ "OPR", op.val });
	}
}

static void parse_condition() {
	if (expect("keyword", "odd")) {
		parse_expression();
		emit({ "OPR", "odd" });
		return;
	}
	// LHS
	parse_expression();
	// conditional
	static const vector<string> OPER = { "=","!=","<",">","<=",">=" };
	int found = 0;
	for (auto& o : OPER)
		if (expect("operator", o)){  found = 1; break;  }
	if (!found)  throw string("condition: expected comparison operator");
	auto& op = lasttok();
	// RHS
	parse_expression();
	// calculate conditional
	emit({ "OPR", op.val });
}

static void parse_statement() {
	// printf("parsing statement...\n");
	// assign
	if (expect("identifier", "")) {
		auto& id = lasttok();
		require("operator", ":=");
		parse_expression();
		emit({ "STO", id.val });
	}
	// call
	if (expect("keyword", "call")) {
		require("identifier", "");
		auto& id = lasttok();
		emit({ "CAL", id.val });
	}
	// begin / end block
	if (expect("keyword", "begin")) {
		parse_statement();
		while (expect("operator", ";"))
			parse_statement();
		require("keyword", "end");
	}
	// if
	if (expect("keyword", "if")) {
		parse_condition();
		// insert jump command
		int jmppos = prog.size();
		emit({ "JPC", "-1" });
		// if body
		require("keyword", "then");
		parse_statement();
		// fix jump command
		prog[jmppos].b = prog.size();
	}
	// while
	// read / write
	if (expect("keyword", "read") || expect("keyword", "write")) {
		auto& cmd = lasttok().val;
		require("identifier", "");
		auto& id = lasttok();
		if (cmd == "write")  emit({ "LOD", id.val });
		emit({ "EXT", cmd });
		if (cmd == "read" )  emit({ "STO", id.val });
	}
	// null - actually ok
}

static void parse_block() {
	emit({ "_block" });
	// const block
	if (expect("keyword", "const")) {
		// printf("parsing const block...\n");
		while (true) {
			require("identifier", "");
			auto& id = lasttok();
			require("operator", "=");
			require("number", "");
			auto& num = lasttok();
			emit({ "_const", id.val, num.val });
			if (!expect("operator", ","))  break;
		}
		require("operator", ";");
	}
	// var block
	if (expect("keyword", "var")) {
		// printf("parsing var block...\n");
		vector<string> varlist = { "_varlist" };
		while (true) {
			require("identifier", "");
			varlist.push_back(lasttok().val);
			if (!expect("operator", ","))  break;
		}
		require("operator", ";");
		emit(varlist);
	}
	// insert jump to main block
	int jmppos = prog.size();
	emit({ "JMP", "-1" });
	// procedure block list
	while (expect("keyword", "procedure")) {
		// printf("parsing procedure block...\n");
		require("identifier", "");
		auto& id = lasttok();
		emit({ "_procedure", id.val });
		require("operator", ";");
		parse_block();
		require("operator", ";");
	}
	// fix jump command
	prog[jmppos].b = prog.size();
	// statement
	parse_statement();
	emit({ "_end" });
}

static void parse_program() {
	parse_block();
	require("operator", ".");
	prog.pop_back();  // delete final block return (program ends)
}

//-- main parse call

int parse() {
	try {
		parse_program();
		printf("parse OK\n");
	}
	catch (const string& err) {
		fprintf(stderr, 
			"parser error:\n"
			"	%s\n"
			"	line: %d\n"
			"	at: [%s] [%s]\n", 
			err.c_str(), nexttok().line, nexttok().type.c_str(), nexttok().val.c_str() );
		return 1;
	}
	return 0;
}

}