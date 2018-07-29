#pragma once
#include "helpers.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace pl0 {
using namespace std;

// consts
static const vector<string> KEYWORDS = { 
	"const","var","procedure","call","begin","end","if","then","else","while","do","odd","read","write" };
	
// token storage
struct Node {
	std::string val, type;
	int line;
};
vector<Node> toklist;
// parse globals
static stringstream ss;
static int line_no = 0;

// get simple symbols
static string get_number_posint() {
	string s;
	if (is_numeric(ss.peek()))
		while (is_numeric(ss.peek()))  s += ss.get();
	else
		throw "expected 0-9 in get_number";
	return s;
}
static void get_number() {
	// get number and put it into toklist
	string s;
	if (ss.peek() == '+' || ss.peek() == '-')  s += ss.get();
	s += get_number_posint();
	if (ss.peek() == '.') {
		s += ss.get();
		s += get_number_posint();
	}
	toklist.push_back({ s, "number", line_no });
}
static void get_identifier() {
	string s;
	if (!is_alpha(ss.peek()))  throw "expected alpha character in get_ident";
	while (is_alphanumeric(ss.peek()))  s += ss.get();
	toklist.push_back({ s, "identifier", line_no });
}
static void get_operator() {
	string s;
	char c = ss.peek();
	if (!is_operator(c))  throw "expected operator in get_operator";
	s += ss.get();
	// compound operators
	if      (c==':' && ss.peek()=='=')  s += ss.get();
	else if (c=='>' && ss.peek()=='=')  s += ss.get();
	else if (c=='<' && ss.peek()=='=')  s += ss.get();
	else if (c=='!' && ss.peek()=='=')  s += ss.get();
	toklist.push_back({ s, "operator", line_no });
}

void tokshow() {
	printf("token count: %d\n", pl0::toklist.size());
	for (const auto& t : pl0::toklist)
		printf("	%03d [%s] %s\n", t.line, t.val.c_str(), t.type.c_str());
}

int load(const std::string fname) {
	// open file
	fstream fs(fname, fstream::in);
	if (!fs.is_open()) {
		fprintf(stderr, "error: could not open file %s\n", fname.c_str());
		return 1;
	}
	// parse line by line
	try {
		string ln;
		while (getline(fs, ln)) {
			line_no++;
			ss = stringstream(ln);
			ss >> ws;
			char c;
			while ((c = ss.peek()) != EOF) {
				if      (is_numeric(c))  get_number();
				else if (is_alpha(c))  get_identifier();
				else if (is_operator(c))  get_operator();
				else    throw string("unexpected character: [")+c+"]";
				ss >> ws;
			}
		}
	}
	catch (const string& err) {
		fprintf(stderr, 
			"tokenizer error:\n"
			"	%s\n"
			"	line: %d\n", 
			err.c_str(), line_no );
		return 1;
	}
	// identify keywords
	for (auto& tok : toklist) {
		if (tok.type != "identifier")  continue;
		for (const auto& k : KEYWORDS)
			if (tok.val == k) { tok.type = "keyword";  break; }
	}
	// OK
	printf("load OK\n");
	return 0;
}

}