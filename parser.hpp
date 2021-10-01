#pragma once

#include "lexer.hpp"

enum node_kind_t  { NODE_OP = 30, NODE_VAL , NODE_VAR};

struct node_data_t {
	enum node_kind_t k;
	union {
		enum operation_t op;
		int  val;
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

node_t *syntax_tree_t::parse_term(lex_array_t &lex_array) {
	node_t *new_node;
	switch (lex_array.lexems_[state_].kind) {
		case BRAC:
			if (lex_array.lexems_[state_].lex.b == LBRAC)
				++state_;
			else {
				std::cout << "Syntax error: unexpected bracket type - expected \'(\'\n" ;
				abort();
			}
			new_node = parse_expr(lex_array);
			if (lex_array.lexems_[state_].lex.b == RBRAC)
				++state_;
			else {
				std::cout << "Syntax error: unexpected bracket type - expected \')\'\n" ;
				abort();
			}
			return new_node;
			break;
		
		case VAR:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAR;
			new_node->data.u.var = lex_array.lexems_[state_++].lex.var;
			return new_node;
			break;
		
		case NOT:
			new_node			 =  new node_t();
			new_node->data.k     = NODE_OP;
			new_node->data.u.op  = lex_array.lexems_[state_++].lex.op;
			new_node->left       = parse_term(lex_array);
			new_node->right      = nullptr;
			return new_node;
			break;
		
		case T:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAL;
			new_node->data.u.val = 1;
			new_node->right      = nullptr;
			new_node->left       = nullptr;
			++state_;
			return new_node;
			break;
		
		case F:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAL;
			new_node->data.u.val = 0;
			new_node->right      = nullptr;
			new_node->left       = nullptr;
			return new_node;
			break;
		default:
			return nullptr;
	}
}

node_t *syntax_tree_t::parse_conjunct(lex_array_t &lex_array) {
	return NULL;
}

node_t *syntax_tree_t::parse_disjunct(lex_array_t &lex_array) {
	return NULL;
}

node_t *syntax_tree_t::parse_expr(lex_array_t &lex_array) {
	return NULL;
}