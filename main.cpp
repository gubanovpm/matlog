#include "parser.hpp"

// TODO: abort - concentrate evil
// TODO: incapsulate brooo - you in c++
// TODO: ??? 

int main () {


	char str[256] = {};
	fgets(str, 100, stdin);

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
	result.destroy_syntax_tree_t(result.root_);
	return 0;
}