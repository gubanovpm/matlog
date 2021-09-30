#include "lexer.hpp"
#include <string>
int main () {
	
	lex_array_t array("(v1 -> v2) & v3");
	for (int i = 0; i < 10; ++i) {
		std::cout << array.lexems[i].lex ;
	}
	return 0;
}