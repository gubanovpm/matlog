#pragma once

#include "lexer.hpp"

/* Grammar:
	expr = 

	term = not(expr)| (expr) | variable

*/

enum node_kind_t  { NODE_OP = 30, NODE_VAL };

struct node_data_t {
	enum node_kind_t k;
	union {
		enum operation_t op;
		char *var;
	} u;
	
};

struct node_t {
	struct node_t *left, *right;
	struct node_data_t data;
};

struct syntax_tree_t {
	node_t *root;

	syntax_tree_t(lex_array_t &lex_array);
};

syntax_tree_t::syntax_tree_t(lex_array_t &lex_array) {
	node_t *root = nullptr;


}