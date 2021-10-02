#include "parser.hpp"

int main () {

	lex_array_t array("~(~(v1 & v2)->v3|v4)\0");
	
	std::cout << "lexer result: " ;
	for (int i = 0; i < array.size_; ++i) {
		std::cout << "|" << array.lexems_[i] << "|   " ;
	}
	std::cout << std::endl;
	//std::cout << "i was here!\n" ;
	syntax_tree_t result(array);
	
	std::cout << "parser result: " ;
	result.show();
	return 0;
}