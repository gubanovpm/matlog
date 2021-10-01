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

void print_node(node_t *node);

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
	state_ = 0;
	root = parse_expr(lex_array);
}

node_t *syntax_tree_t::parse_term(lex_array_t &lex_array) {
	node_t *new_node;
	std::cout << state_ << "\n";
	std::cout << lex_array.lexems_[state_].kind << std::endl;
	switch (lex_array.lexems_[state_].lex.op) {
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
			print_node(new_node);
			return new_node;
		
		case VAR:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAR;
			new_node->data.u.var = lex_array.lexems_[state_++].lex.var;
			new_node->right      = nullptr;
			new_node->left       = nullptr;
			print_node(new_node);
			return new_node;
		
		case NOT:
			std::cout << "i was here!\n" ;
			new_node			 = new node_t();
			new_node->data.k     = NODE_OP;
			new_node->data.u.op  = lex_array.lexems_[state_++].lex.op;
			
			print_node(new_node);

			new_node->left       = parse_term(lex_array);
			new_node->right      = nullptr;
			
			print_node(new_node);
			return new_node;
		
		case T:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAL;
			new_node->data.u.val = 1;
			new_node->right      = nullptr;
			new_node->left       = nullptr;
			++state_;
			
			print_node(new_node);
			return new_node;
		
		case F:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAL;
			new_node->data.u.val = 0;
			new_node->right      = nullptr;
			new_node->left       = nullptr;
			
			print_node(new_node);
			return new_node;
		default:
			std::cout << "Syntax error: unknow term type\n" ;
			abort();
	}
}

node_t *syntax_tree_t::parse_conjunct(lex_array_t &lex_array) {
	node_t *new_node;
	
	new_node = parse_term(lex_array);
	if (lex_array.lexems_[state_++].lex.op != AND) {
		new_node->right  = nullptr;
		new_node->left   = nullptr;
		
		print_node(new_node);
		return new_node;
	}
	new_node->data.k    = NODE_OP;
	new_node->data.u.op = AND;
	new_node->left      = new_node;
	++state_; 
	new_node->right     = parse_conjunct(lex_array);

	print_node(new_node);
	return new_node;
}

node_t *syntax_tree_t::parse_disjunct(lex_array_t &lex_array) {
	node_t *new_node;
	
	new_node = parse_conjunct(lex_array);
	if (state_ < lex_array.size_ && lex_array.lexems_[state_++].lex.op != OR) {
		new_node->right  = nullptr;
		new_node->left   = nullptr;
		print_node(new_node);
		return new_node;
	}
	new_node->data.k    = NODE_OP;
	new_node->data.u.op = OR;
	new_node->left      = new_node;
	++state_; 
	new_node->right     = parse_disjunct(lex_array);

	print_node(new_node);
	return new_node;
}

node_t *syntax_tree_t::parse_expr(lex_array_t &lex_array) {
	node_t *new_node = parse_disjunct(lex_array);

	new_node = parse_disjunct(lex_array);
	if (state_ < lex_array.size_ && lex_array.lexems_[state_++].lex.op != IMPL) {
		new_node->right  = nullptr;
		new_node->left   = nullptr;
		
		print_node(new_node);
		return new_node;
	}
	new_node->data.k    = NODE_OP;
	new_node->data.u.op = IMPL;
	new_node->left      = new_node;
	++state_; 
	new_node->right     = parse_expr(lex_array);

	print_node(new_node);
	return new_node;
}

void print_node(node_t *node) {
	std::cout << "this node is : ";
	switch (node->data.k) {
		case NODE_OP:
			std::cout << "operation " ;
			switch (node->data.u.op) {
				case AND:
					std::cout << "AND" << std::endl;
					break;
				case OR:
					std::cout << "OR" << std::endl;
					break;
				case IMPL:
					std::cout << "IMPL" << std::endl;
					break;
				case NOT:
					std::cout << "NOT" << std::endl;
					break;
			}
			break;
		case NODE_VAL:
			std::cout << "value " << node->data.u.val << std::endl;
			break;
		case NODE_VAR:
			std::cout << "variable " << node->data.u.var << std::endl;
			break;
	}
}