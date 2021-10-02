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
	
};

struct node_t {
	struct node_t *left, *right;
	struct node_data_t data;
};

void print_n(node_t *node);

struct syntax_tree_t {
	node_t *root_ ;
	int     state_;

	void show();
	void print_node(node_t *root);					//private
	syntax_tree_t(lex_array_t &lex_array);
	
	node_t *parse_expr    (lex_array_t &lex_array);
	node_t *parse_disjunct(lex_array_t &lex_array);
	node_t *parse_conjunct(lex_array_t &lex_array);
	node_t *parse_term    (lex_array_t &lex_array);
};

syntax_tree_t::syntax_tree_t(lex_array_t &lex_array) {
	state_ = 0;
	root_ = parse_expr(lex_array);
}

node_t *syntax_tree_t::parse_term(lex_array_t &lex_array) {

	printf("START parsing term\n");
	printf("%p <- adress root_ when state_ = %d\n", root_, state_);
	node_t *new_node;
	if (state_ >= lex_array.size_)
		return nullptr;
	std::cout << state_ << "\n";
	std::cout << lex_array.lexems_[state_].kind << std::endl;
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
			//print_node(new_node);
			printf("EXIT from parse_TERM\n");
			return new_node;
		
		case VAR:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAR;
			new_node->data.u.var = lex_array.lexems_[state_++].lex.var;
			new_node->right      = nullptr;
			new_node->left       = nullptr;
			//print_node(new_node);
			printf("EXIT from parse_TERM\n");
			return new_node;
		case OP:
			if (lex_array.lexems_[state_].lex.op == NOT) {
			//std::cout << "i was here!\n" ;
				new_node			 = new node_t();
				new_node->data.k     = NODE_OP;
				new_node->data.u.op  = lex_array.lexems_[state_++].lex.op;
				
				print_node(new_node);

				new_node->left       = parse_term(lex_array);
				new_node->right      = nullptr;
				
				//print_node(new_node);
			printf("EXIT from parse_TERM\n");
				return new_node;
			}
			return nullptr;
			break;
		
		case T:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAL;
			new_node->data.u.val = 1;
			new_node->right      = nullptr;
			new_node->left       = nullptr;
			++state_;
			
			//print_node(new_node);
			printf("EXIT from parse_TERM\n");
			return new_node;
		
		case F:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAL;
			new_node->data.u.val = 0;
			new_node->right      = nullptr;
			new_node->left       = nullptr;
			++state_;
			
			//print_node(new_node);
			printf("EXIT from parse_TERM\n");
			return new_node;
		default:
			std::cout << "Syntax error: unknow term type\n" ;
			abort();
	}
}

node_t *syntax_tree_t::parse_conjunct(lex_array_t &lex_array) {
	printf("START parse_conjunct\n");
	node_t *new_node;

	new_node = parse_term(lex_array);
	if (state_ >= lex_array.size_ || lex_array.lexems_[state_].lex.op != AND) {
		//print_node(new_node);
		printf("EXIT FROM PARSE_CONJUNCT\n");
		return new_node;
	}

	node_t *temp = new node_t ();
	temp->data.k    = NODE_OP;
	temp->data.u.op = AND;
	temp->left      = new_node;

	++state_;
	temp->right     = parse_conjunct(lex_array);

	//print_node(temp);
		printf("EXIT FROM PARSE_CONJUNCT\n");
	return temp;
}

node_t *syntax_tree_t::parse_disjunct(lex_array_t &lex_array) {
	
	printf("START parsing disjunct\n");
	node_t *new_node;
	
	new_node = parse_conjunct(lex_array);
	if (state_ >= lex_array.size_ || lex_array.lexems_[state_].lex.op != OR) {
		//print_node(new_node);
		printf("EXIT FROM PARSE_DISJUNCT\n");
		return new_node;
	}

	node_t *temp = new node_t();
	temp->data.k    = NODE_OP;
	temp->data.u.op = OR;
	temp->left      = new_node;
	
	++state_; 
	temp->right     = parse_disjunct(lex_array);

	//print_node(temp);
		printf("EXIT FROM PARSE_DISJUNCT\n");
	return temp;
}

node_t *syntax_tree_t::parse_expr(lex_array_t &lex_array) {

	printf("START parsing expr\n");
	node_t *new_node;

	new_node = parse_disjunct(lex_array);
	if (state_ >= lex_array.size_ || lex_array.lexems_[state_].lex.op != IMPL) {
		//print_node(new_node);
		printf("EXIT from parse_expr\n");
		return new_node;
	}

	node_t *temp = new node_t();	
	temp->data.k    = NODE_OP;
	temp->data.u.op = IMPL;
	temp->left      = new_node;

	++state_; 
	temp->right     = parse_expr(lex_array);

//	print_node(temp);
		printf("EXIT from parse_expr\n");
	return temp;
}

void print_n(node_t *node) {
	if (node == nullptr) {
		printf("nullptr node\n");
	}
	
	switch (node->data.k) {
		case NODE_OP:
			switch (node->data.u.op) {
				case AND:
					std::cout << "&" ;
					break;
				case OR:
					std::cout << "|" ;
					break;
				case IMPL:
					std::cout << "->" ;
					break;
				case NOT:
					std::cout << "~" ;
					break;
			}
			break;
		case NODE_VAL:
			std::cout << node->data.u.val ;
			break;
		case NODE_VAR:
			std::cout << node->data.u.var ;
			break;
		default:
			printf("<%d - unknow node type>", node->data.k);
	}
}

void syntax_tree_t::show() {
	print_node(root_);
	std::cout << std::endl;
}

void syntax_tree_t::print_node(node_t *root) {
	if (root != nullptr) {	
		print_node(root->left);
		print_n(root);
		print_node(root->right);
	}
}