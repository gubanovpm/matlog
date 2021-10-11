#pragma once

#include "lexer.hpp"

enum node_kind_t  { NODE_OP = 41, NODE_VAL , NODE_VAR};

struct node_data_t {
	enum node_kind_t k;
	union {
		enum operation_t op;
		int  val;
		char *var;
	} u;
	char value = -1;
};

struct node_t {
	struct node_t      *left, *right;
	struct node_data_t data;
	bool               isbracket = false;

	node_t();
};

void print_n(node_t *node);
node_t *copy_node(node_t *copied_node);

struct syntax_tree_t {
	node_t *root_ ;
	int     state_;

	syntax_tree_t();
	syntax_tree_t(lex_array_t &lex_array);
	syntax_tree_t(node_t *new_root);
	~syntax_tree_t();
	void destroy_syntax_tree_t(node_t *node);
	
	node_t *parse_expr     (lex_array_t &lex_array);
	node_t *parse_disjunct (lex_array_t &lex_array);
	node_t *parse_conjunct (lex_array_t &lex_array);
	node_t *parse_term     (lex_array_t &lex_array);

	node_t *copy_tree_root (node_t *root);

	void show();
	void print_node(node_t *root);					//private
};