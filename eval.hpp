#pragma once

#include <unordered_map>

#include "lexer.hpp"
#include "parser.hpp"

struct eval() {
	std::unordered_map <std::string, bool> variables_table_;
	syntax_tree_t *form_tree_;

	eval (syntax_tree_t copied_tree, std::string arg);
	~eval();
}

eval::eval(syntax_tree_t copied_tree, std::string arg) {
	form_tree_            = new syntax_tree_t();
	form_tree_.state_     = copied_tree.state_ ;
	form_tree_.root_      = copied_tree.copy_tree();
	form_tree_.variables_ = copied_tree.variables_;

	lex_array_t variable_lexem(arg.data);
	
}