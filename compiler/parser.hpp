#pragma once
#include "tokenizer.hpp"

namespace pl0 {
using namespace std;

static int pos = 0;

//-- next token functions
static const Node& nexttok() {
	static const Node nEOF = { "EOF", "EOF", -1 };
	if (pos >= (int)toklist.size())  return nEOF;
	return toklist[pos];
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
	if      (expect("identifier", "")) { }
	else if (expect("number", "")) { }
	else if (expect("operator", "(")) { parse_expression();  require("operator", ")"); }
	else 	{ throw string("unexpected token in factor"); }
}

static void parse_term() {
	parse_factor();
	while (expect("operator", "*") || expect("operator", "/"))
		parse_factor();
}

static void parse_expression() {
	// throw string("unimplemented: expression");
	expect("operator", "+") || expect("operator", "-");  // either or none
	parse_term();
	while (expect("operator", "+") || expect("operator", "-"))
		parse_term();
}

static void parse_statement() {
	// printf("parsing statement...\n");
	// assign
	if (expect("identifier", "")) {
		require("operator", ":=");
		parse_expression();
	}
	// call
	if (expect("keyword", "call")) {
		require("identifier", "");
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
		require("identifier", "");
	}
	// null - actually ok
}

static void parse_block() {
	// const block
	if (expect("keyword", "const")) {
		// printf("parsing const block...\n");
		while (true) {
			require("identifier", "");
			require("operator", "=");
			require("number", "");
			if (!expect("operator", ","))  break;
		}
		require("operator", ";");
	}
	// var block
	if (expect("keyword", "var")) {
		// printf("parsing var block...\n");
		while (true) {
			require("identifier", "");
			if (!expect("operator", ","))  break;
		}
		require("operator", ";");
	}
	// procedure block list
	while (expect("keyword", "procedure")) {
		// printf("parsing procedure block...\n");
		require("identifier", "");
		require("operator", ";");
		parse_block();
		require("operator", ";");
	}
	// statement
	parse_statement();
}

static void parse_program() {
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