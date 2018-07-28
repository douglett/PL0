#pragma once
#include <string>
#include <vector>

namespace pl0 {

inline int is_alpha(char c) {
	return ((c>='a' && c<='z') || (c>='A' && c<='Z'));
}
inline int is_numeric(char c) {
	return (c>='0' && c<='9');
}
inline int is_alphanumeric(char c) {
	return is_alpha(c) || is_numeric(c);
}
inline int is_innumber(char c) {
	return (is_numeric(c) || c=='+' || c=='-' || c=='.');
}
inline int is_operator(char c) {
	static const std::string OP = "=><!*/+-,;:.()";
	for (char o : OP)
		if (o == c)  return 1;
	return 0;
}

inline std::string join(const std::vector<std::string>& vs) {
	std::string s;
	for (int i = 0; i < (int)vs.size(); i++)
		s += (i > 0 ? " " : "") + vs[i];
	return s;
}

}