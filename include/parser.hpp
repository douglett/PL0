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

// static int find_level(const string& id) {
// 	for (int i = table.size()-1; i >= 0; i--)
// 		for (int j = table[i].size()-1; j >= 0; j--)
// 			if (table[i][j].first == id)  return block_level - table[i][j].second;
// 	throw string("find_level: var not defined: "+id);
// }
// static string find_level_str(const string& id) {
// 	return to_string(find_level(id));
// }

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
	// while
	// read / write
	if (expect("keyword", "read") || expect("keyword", "write")) {
		auto& cmd = lasttok().val;
		require("identifier", "");
		auto& id = lasttok();
		emit({ "EXT", cmd, id.val });
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
			// auto& id = lasttok();
			// emit({ "VAR", id.val });
			varlist.push_back(lasttok().val);
			if (!expect("operator", ","))  break;
		}
		require("operator", ";");
		emit(varlist);
	}
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
	// statement
	parse_statement();
	emit({ "_end" });
}

static void parse_program() {
	// emit({ "JMP",  })
	parse_block();
	require("operator", ".");
}

//-- main parse call

int parse() {
	try {
		parse_program();
		printf("parse OK\n");
	}
	catch (const string& err) {
		// int line = ( pos >= (int)toklist.size() ? -1 : toklist[pos].line );
		fprintf(stderr, 
			"parser error:\n"
			"	%s\n"
			"	line: %d\n"
			"	at: [%s] [%s]", 
			err.c_str(), nexttok().line, nexttok().type.c_str(), nexttok().val.c_str() );
		return 1;
	}
	return 0;
}

}