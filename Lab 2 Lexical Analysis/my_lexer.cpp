/*#include <regex>
#include <string>
#include <iostream>
#include <iterator>
#include <fstream> 

using namespace std;

int main() {

	ifstream fin("programPascal.txt");
	if (!fin) {
		cerr << "Could not open file.\n";
		return 1;
	}
	string code((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
	vector<pair<string, regex>> patterns = {
	   {"KEYWORD",		regex(R"(\b(program|unit|uses|integer|real|boolean|char|string|nil|const|type|var|procedure|function|)"
							  R"(begin|end|if|then|else|case|of|while|do|for|to|downto|repeat|until|with|record|array|set|file|)"
							  R"(div|mod|not|and|or|xor|write|writeln|read|readln|true|false|in)\b)",
							  regex::icase)
		},
	   {"IDENTIFIER",   regex(R"(\b[A-Za-z_][A-Za-z0-9_]*\b)")},
	   {"NUMBER",		regex(R"(\$[0-9A-Fa-f]+)")},
       {"NUMBER",		regex(R"([0-9]+\.[0-9]+([eE][+-]?[0-9]+)?)")},
       {"NUMBER",		regex(R"([0-9]+([eE][+-]?[0-9]+))")},
       {"NUMBER",		regex(R"([0-9]+)")},
	   {"STRING",		regex(R"('([^']|'')*'|#[0-9]+)")},
	   {"PREPROCESSOR_DIRECTIVE", regex(R"((\{\$[^}]*\}|\(\*\$[\s\S]*?\*\)))")},
	   {"COMMENT",		regex(R"(\{[^}]*\}|\(\*[\s\S]*?\*\)|\/\/[^\n]*)")},
	   {"OPERATOR",		regex(R"(:=|<>|<=|>=|\.\.|\+|\-|\*|\/|=|<|>|\^|@)")},
	   {"PUNCTUATION",  regex(R"(;|,|:|\.|\(|\)|\[|\])")},
	   {"WS",      regex("[ \t\r\n]+")}
	};
	string :: const_iterator it = code.begin();
	while (it != code.end()) {
		bool matched = false;

		for (auto& p : patterns) {
			smatch m;
			if (regex_search(it, code.cend(), m, p.second, regex_constants::match_continuous)) {
				if (p.first != "WS") {
					cout << "<" << m.str() << ", " << p.first << ">\n";
				}
				it += m.length();
				matched = true;
				break;
			}
		}

		if (!matched) {
			string err(1, *it);
			cout << "<" << err << ", ERROR>\n";
			++it;
		}
	}
	return 0;
}
*/