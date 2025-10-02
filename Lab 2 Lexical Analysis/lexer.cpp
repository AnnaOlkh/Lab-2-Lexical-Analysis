#include <regex>
#include <string>
#include <iostream>

using namespace std;

int main() {
	string code = "x := -42; y := +3.14; z := -1e-3; h := $DEAD; k := -$2A;";
	regex number_pattern(R"(-?(?:\$[0-9A-Fa-f]+|[0-9]+\.[0-9]+(?:[eE][+-]?[0-9]+)?|[0-9]+(?:[eE][+-]?[0-9]+)|[0-9]+))");
	auto code_begin = sregex_iterator (code.begin(), code.end(), number);
	auto code_end = sregex_iterator();
    for (auto it = code_begin; it != code_end; ++it)
        std::cout << it->str() << "\n";
	return 0;
}