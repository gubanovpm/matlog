#pragma once

#include <unordered_map>
#include <list>

#include "lexer.hpp"
#include "parser.hpp"

struct eval_t {
	std::unordered_map < std::string, std::list < node_t * > > pointer_table_;
	syntax_tree_t *form_tree_;

	eval_t (syntax_tree_t &copied_tree, std::string arg);
	void parse_eval(node_t *current);
	~eval_t();
};

eval_t::eval_t(syntax_tree_t &copied_tree, std::string arg) {
	form_tree_         = new syntax_tree_t();
	form_tree_->state_ = copied_tree.state_ ;
	form_tree_->root_  = copied_tree.copy_tree_root(copied_tree.root_, pointer_table_);

	lex_array_t lexems (arg.data());
	int state = 0;
	while (state < lexems.size_ - 2) {
		switch (lexems.lexems_[state].kind) {
			case VAR:
				if (lexems.lexems_[++state].kind == OP && lexems.lexems_[++state].lex.op == EQUAL)
					++state;
				else {
					std::cout << "eval - unexpected lexem" << 
							 ": expected EQUAL operation"<< std::endl;
					abort();
				}
				if (lexems.lexems_[state].kind == T || lexems.lexems_[state].kind == F) {
					std::list < node_t * > cur_list = pointer_table_[std::string(lexems.lexems_[state - 2].lex.var)] ;
					for (auto it : cur_list) {
						it->data.k     = NODE_VAL;
						it->data.u.val = lexems.lexems_[state].lex.value;
						it->data.value = lexems.lexems_[state].lex.value;
					}
				}
				else {
					std::cout << "eval - unexpected lexem" << 
							 ": expected true or false value"<< std::endl;
					abort();
				}
				break;
			default:
				std::cout << "eval lexer error - unexpected lexem" <<
							 ": expected VariableName"<< std::endl;
				abort();
		}
	}
	form_tree_->show();
}

void eval_t::parse_eval (node_t *current) {
	
}

eval_t::~eval_t() {
}