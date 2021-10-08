#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "eval.hpp"

// TODO: just killing - before abort() delete all alocate memory
// TODO: incapsulate brooo - add private and public methods plz
// TODO: kings don't rule forever, my son - add inheritance in parser and lexer

int main () {

	char str[256] = {};
	if (fgets(str, 255, stdin) == nullptr)
		abort();

	std::cout << "TEST : " << str << std::endl;
	lex_array_t array((const char *)(str));
	
	std::cout << "lexer  result: " ;
	for (int i = 0; i < array.size_; ++i) {
		std::cout << "< " << array.lexems_[i] << " >   " ;
	}
	std::cout << std::endl;
	syntax_tree_t result(array);
	
	std::cout << "parser result: " ;
	result.show();

	std::cout << "eval   result: " ;
	eval_t whoami(result);

	std::cout << "is form TAUT?: " << whoami.is_TAUT() << std::endl;
	std::cout << "is form SAT ?: " << whoami.is_SAT () << std::endl;
	return 0;
}