#pragma once
#include "helpers.hpp"
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace pl0 {
using namespace std;

struct Node {
	std::string val;
	int line;
	std::string type;
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
	toklist.push_back({ s, line_no, "number" });
}
static void get_identifier() {
	string s;
	if (!is_alpha(ss.peek()))  throw "expected alpha character in get_ident";
	while (is_alphanumeric(ss.peek()))  s += ss.get();
	toklist.push_back({ s, line_no, "identifier" });
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
	toklist.push_back({ s, line_no, "operator" });
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
	catch (string s) {
		fprintf(stderr, 
			"token parse error:\n"
			"	%s\n"
			"	line: %d\n", 
			s.c_str(), line_no );
		return 1;
	}

	printf("load OK\n");
	return 0;
}

}