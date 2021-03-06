#include <iostream>
#include <list>

#include "lexer.hpp"
#include "parser.hpp"

node_t::node_t() {
	left  = nullptr ;
	right = nullptr ;
	isbracket  = false ;
	data.value = -1;
}

syntax_tree_t::syntax_tree_t() {
	state_ = 0;
	root_  = nullptr;
}

syntax_tree_t::syntax_tree_t(node_t *new_root) {
	state_ = 0;
	root_  = new_root;
}

syntax_tree_t::syntax_tree_t(lex_array_t &lex_array) {
	state_ = 0;
	root_ = nullptr;
	root_ = parse_expr(lex_array);

	if (state_ != lex_array.size_) {
		printf("strange input constructions!\n");
		std::cout << lex_array.size_ << std::endl;
		destroy_syntax_tree_t(root_);
		root_ = nullptr;
		return;
	}
}

syntax_tree_t::syntax_tree_t(const syntax_tree_t &other) {
	root_ = other.root_;
}

node_t *syntax_tree_t::parse_term(lex_array_t &lex_array) {
	node_t *new_node = nullptr;
	if (state_ >= lex_array.size_)
		return nullptr;
	switch (lex_array.lexems_[state_].kind) {
		case BRAC:
			if (lex_array.lexems_[state_].lex.b == LBRAC) {
				++state_;
			}
			else {
				std::cout << "Syntax error: unexpected bracket type - expected \'(\'\n" ;
				state_ = lex_array.size_ + 1;
				return nullptr;
			}

			new_node = parse_expr(lex_array);
			if (new_node != nullptr)
				new_node->isbracket = true;

			if (lex_array.lexems_[state_].lex.b == RBRAC) {
				++state_;
			}
			else {
				std::cout << "Syntax error: unexpected bracket type - expected \')\'\n" ;
				state_ = lex_array.size_ + 1;
				return nullptr;
			}
			return new_node;
		
		case VAR:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAR;
			new_node->data.u.var = lex_array.lexems_[state_++].lex.var;

			return new_node;
		case OP:
			if (lex_array.lexems_[state_].lex.op == NOT) {
				new_node			 = new node_t();
				new_node->data.k     = NODE_OP;
				new_node->data.u.op  = lex_array.lexems_[state_++].lex.op;
				
				new_node->left       = parse_term(lex_array);
				
				return new_node;
			}
			printf("Operation does not exist!\n");
			state_ = lex_array.size_ + 1;
			return nullptr;
		
		case T:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAL;
			new_node->data.u.val = 1;
			new_node->data.value = 1;
			++state_;
			
			return new_node;
		
		case F:
			new_node             = new node_t();
			new_node->data.k     = NODE_VAL;
			new_node->data.u.val = 0;
			new_node->data.value = 0;
			++state_;
			
			return new_node;
		default:
			std::cout << "Syntax error: unknow term type\n" ;
			state_ = lex_array.size_ + 1;
			return nullptr;
	}
}

node_t *syntax_tree_t::parse_conjunct(lex_array_t &lex_array) {
	node_t *new_node = nullptr;

	new_node = parse_term(lex_array);
	if (state_ >= lex_array.size_ || lex_array.lexems_[state_].lex.op != AND) {
		return new_node;
	}

	node_t *temp     = new node_t ();
	temp->data.k     = NODE_OP;
	temp->data.u.op  = AND;
	temp->left       = new_node;
	temp->isbracket  = false;

	++state_;
	temp->right     = parse_conjunct(lex_array);

	return temp;
}

node_t *syntax_tree_t::parse_disjunct(lex_array_t &lex_array) {
	
	node_t *new_node;
	
	new_node = parse_conjunct(lex_array);
	if (state_ >= lex_array.size_ || lex_array.lexems_[state_].lex.op != OR) {
		return new_node;
	}

	node_t *temp    = new node_t();
	temp->data.k    = NODE_OP;
	temp->data.u.op = OR;
	temp->left      = new_node;
	temp->isbracket = false;
	
	
	++state_; 
	temp->right     = parse_disjunct(lex_array);

	return temp;
}

node_t *syntax_tree_t::parse_expr(lex_array_t &lex_array) {

	node_t *new_node;

	new_node = parse_disjunct(lex_array);
	if (state_ >= lex_array.size_ || lex_array.lexems_[state_].lex.op != IMPL) {
		return new_node;
	}

	node_t *temp = new node_t();	
	temp->data.k    = NODE_OP;
	temp->data.u.op = IMPL;
	temp->left      = new_node;
	temp->isbracket = false;

	++state_; 
	temp->right     = parse_expr(lex_array);

	return temp;
}

void print_n(node_t *node) {
	if (node == nullptr) {
		printf("nullptr node\n");
	}
	
	switch (node->data.k) {
		case NODE_OP:
			switch (node->data.u.op) {
				case AND:   std::cout << "&"  ; break;
				case OR:    std::cout << "|"  ; break;
				case IMPL:  std::cout << "->" ; break;
				case NOT:   std::cout << "~"  ; break;
				case EQUAL: std::cout << "="  ; break;
				default:    std::cout << "parser error\n" ; return; 
			}
			break;
		case NODE_VAL: std::cout << node->data.u.val ; break;
		case NODE_VAR: std::cout << node->data.u.var ; break;
		default:       printf("<%d - unknow node type>", node->data.k);
	}
}

void syntax_tree_t::show() {
	print_node(root_);
	std::cout << std::endl;
}

void syntax_tree_t::destroy_syntax_tree_t(node_t *node) {
	if (node == nullptr) return;

	destroy_syntax_tree_t(node->left );
	destroy_syntax_tree_t(node->right);

	delete node;
	return;
}

void syntax_tree_t::print_node(node_t *root) {
	if (root == nullptr) return;

	if (root->data.k == NODE_OP && root->data.u.op == NOT) {
		std::cout << "~" ;
		print_node(root->left);
		return;
	}
	else if (root->isbracket) {
		std::cout << "(" ;
		print_node(root->left);
		print_n(root);
		print_node(root->right);
		std::cout << ")" ;
	} else {
		print_node(root->left);
		print_n(root);
		print_node(root->right);
	}
}

node_t *copy_node(node_t *copied_node) {
	if (copied_node == nullptr)
		return nullptr;
	node_t *new_node = new node_t() ;
	new_node->right = copy_node(copied_node->right);
	new_node->left  = copy_node(copied_node->left );

	new_node->data.k     = copied_node->data.k;
	new_node->data.u     = copied_node->data.u;
	new_node->data.value = copied_node->data.value;

	return new_node;
}

node_t *syntax_tree_t::copy_tree_root (node_t *root) {
	return copy_node(root);
}

syntax_tree_t::~syntax_tree_t() {
	destroy_syntax_tree_t(root_);
}

void destroy_subtree(node_t *current) {
	if (current == nullptr) return;

	destroy_subtree(current->left );
	destroy_subtree(current->right);

	delete current;
}