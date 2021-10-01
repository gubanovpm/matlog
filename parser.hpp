#pragma once

#include "lexer.hpp"

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
	node_t *root_ ;
	int     state_;

	syntax_tree_t(lex_array_t &lex_array);
	
	node_t *parse_expr    (lex_array_t &lex_array);
	node_t *parse_disjunct(lex_array_t &lex_array);
	node_t *parse_conjunct(lex_array_t &lex_array);
	node_t *parse_term    (lex_array_t &lex_array);
};

syntax_tree_t::syntax_tree_t(lex_array_t &lex_array) {
	node_t *root = nullptr;
}

node_t *syntax_tree_t::parse_expr(lex_array_t &lex_array) {

}