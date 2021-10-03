#include "parser.hpp"

int main () {


	char str[256] = {};
	scanf("%100s", str);

	std::cout << "TEST : " << str << std::endl;
	lex_array_t array((const char *)(str));
	
	std::cout << "lexer result: " ;
	for (int i = 0; i < array.size_; ++i) {
		std::cout << "<" << array.lexems_[i] << ">\t" ;
	}
	std::cout << std::endl;
	//std::cout << "i was here!\n" ;
	syntax_tree_t result(array);
	
	std::cout << "parser result: " ;
	result.show();
	return 0;
}