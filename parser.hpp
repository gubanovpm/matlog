#pragma once

#include "lexer.hpp"

/* Grammar:
	<expr>     ::= "NOT" (<expr>)
	           |   <implicat> "IMPL" <implicat>
	           |   <implicat>

	<implicat> ::= <disjunct> "IMPL" <implicat>
	           |   "NOT" (<implicat>)
			   |   <disjunct>

	<disjunct> ::= <conjunct> "AND" <disjunct>
               |   "NOT" (<disjunct>)
			   |   <conjunct>

	<conjunct> ::= <term> "OR" <conjunct>
	           |   "NOT" (<conjunct>)
			   |   <term>

	<term>     ::= (<expr>)
	           |   "variable_name"
			   |   "true"
			   |   "false"
			   |   "NOT" <term>
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