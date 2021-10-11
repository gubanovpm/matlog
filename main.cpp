#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "eval.hpp"
#include "cnf.hpp"

// TODO: just killing - before abort() delete all alocate memory
// TODO: incapsulate brooo - add private and public methods plz
// TODO: kings don't rule forever, my son - add inheritance in parser and lexer

int main () {

	char str[256] = {};
	if (fgets(str, 255, stdin) == nullptr)
		abort();

	std::cout << "TEST : " << str << std::endl;
	lex_array_t array((const char *)(str));
	if (array.lexems_ == nullptr) return 0;
	
	std::cout << "lexer  result: " ;
	for (int i = 0; i < array.size_; ++i) {
		std::cout << "< " << array.lexems_[i] << " >   " ;
	}
	std::cout << std::endl;
	syntax_tree_t result(array);
	if (result.root_ == nullptr) return 0;
	std::cout << "parser result: " ;
	result.show();

	std::cout << "eval   result: " ;
	eval_t whoami(result);
	syntax_tree_t end = whoami.evaluation("v1 = 1  v2 = 0");
	end.show();
/*
	std::cout << "is form TAUT?: " << whoami.is_TAUT() << std::endl;
	std::cout << "is form SAT ?: " << whoami.is_SAT () << std::endl;

	syntax_tree_t *cnf = whoami.cnf_form(&result);
	
	cnf_t *cnf_2 = new cnf_t ();
	create_cnf_(cnf_2, cnf->root_);

	cnf_2->print();

	std::cout << DPLL(*cnf_2) << std::endl;
*/
	return 0;
}