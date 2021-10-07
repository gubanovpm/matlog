#pragma once

#include <unordered_map>
#include <list>

#include "lexer.hpp"
#include "parser.hpp"

struct eval_t {
	std::unordered_map < std::string, std::list < node_t * > > pointer_table_;
	syntax_tree_t *form_tree_;

	eval_t (syntax_tree_t &copied_tree, std::string arg);
	node_t *parse_eval(node_t *current);
	~eval_t();
};

eval_t::eval_t(syntax_tree_t &copied_tree, std::string arg) {
	form_tree_         = new syntax_tree_t();
	form_tree_->state_ = copied_tree.state_ ;
	form_tree_->root_  = copied_tree.copy_tree_root(copied_tree.root_, pointer_table_);

	lex_array_t lexems (arg.data());

	std::cout << "lexems: " ;
	for (int i = 0; i < lexems.size_ ; ++i) {
		std::cout << "< " << lexems.lexems_[i] << " >   " ;
	}

	std::cout << " | after all: " ;

	int state = 0;
	while (state < lexems.size_ - 2) {
		switch (lexems.lexems_[state].kind) {
			case VAR:
				++state;
				if (lexems.lexems_[state].kind == OP && lexems.lexems_[state].lex.op == EQUAL)
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
				++state;
				break;
			default:
				std::cout << lexems.lexems_[state] << "\t" ;
				std::cout << "eval lexer error - unexpected lexem" <<
							 ": expected VariableName"<< std::endl;
				abort();
		}
	}
	form_tree_->root_ = parse_eval (form_tree_->root_) ;
	form_tree_->show();
}

node_t *eval_t::parse_eval (node_t *current) {
	if (current         == nullptr) return nullptr;
	if (current->data.k != NODE_OP) return current;
	
	if (current->data.u.op == NOT) {
		node_t *temp = parse_eval(current->left);
		if (temp->data.k == NODE_OP && temp->data.u.op == NOT) {
			return temp->left;
		}
		if (temp->data.k != NODE_VAL) {
			current->left = temp;
			return current;
		}
		node_t *new_node = new node_t ();
		new_node->left   = nullptr;
		new_node->right  = nullptr;
		new_node->data.k = NODE_VAL;
		new_node->data.u.val = 1 - temp->data.u.val;
		new_node->data.value = new_node->data.u.val ;
		new_node->isbracket  = true;
		
		return new_node;
	}

	node_t *temp1 = parse_eval(current->left), *temp2 =  parse_eval(current->right) ;

	if (temp1->data.k != NODE_VAL || temp2->data.k != NODE_VAL) {

		if (current->data.u.op == AND && (temp1->data.value == 0 || temp2->data.value == 0)) {
			node_t *new_node = new node_t ();
			new_node->left   = nullptr;
			new_node->right  = nullptr;
			new_node->data.k = NODE_VAL;
			new_node->data.u.val = 0;
			new_node->data.value = 0;
			new_node->isbracket  = true;

			return new_node;
		}

		if (current->data.u.op == AND && (temp1->data.value == 1 || temp2->data.value == 1)) {
			if (temp1->data.value == 1) return temp2;
				else return temp1;
		}

		if ((current->data.u.op == OR   && (temp1->data.value == 1 || temp2->data.value == 1)) ||
			(current->data.u.op == IMPL && (temp1->data.value == 0 || temp2->data.value == 1))) {
				node_t *new_node = new node_t ();
				new_node->left   = nullptr;
				new_node->right  = nullptr;
				new_node->data.k = NODE_VAL;
				new_node->data.u.val = 1;
				new_node->data.value = 1;
				new_node->isbracket  = true;

				return new_node;
		}

		if (current->data.u.op == OR && (temp1->data.value == 0 || temp2->data.value == 0)) {
			if (temp1->data.value == 0)
				return temp2;
			else
				return temp1;
		}
		
		if (current->data.u.op == IMPL && temp1->data.value == 1) {
			return temp2;
		}

		if (current->data.u.op == IMPL && temp2->data.value == 0) {
			node_t *new_node    = new node_t();
			new_node->left      = temp1;
			new_node->right     = nullptr;
			new_node->data.k    = NODE_OP;
			new_node->data.u.op = NOT;
			temp1->isbracket    = true;

			return new_node;
		}

		current->left  = temp1;
		current->right = temp2;
		current->isbracket  = true;
		return current;
	}

	node_t *new_node = new node_t ();
	new_node->left   = nullptr;
	new_node->right  = nullptr;
	new_node->data.k = NODE_VAL;

	switch (current->data.u.op) {
		case AND:	
			new_node->data.u.val = ((temp1->data.u.val) & (temp2->data.u.val));
			break;

		case OR:
			new_node->data.u.val = (temp1->data.u.val) | (temp2->data.u.val);
			break;

		case IMPL:
			if (temp1->data.u.val && !temp2->data.u.val) new_node->data.u.val = 0;
				else new_node->data.u.val = 1;
			break;	
		default:
			std::cout << "unknown operation" << std::endl;
			abort();
	}
	new_node->data.value = new_node->data.u.val ; 
	new_node->isbracket  = true;
	//form_tree_->destroy_syntax_tree_t(current);
	current = new_node;
	//print_n(current);
	return new_node;
}

eval_t::~eval_t() {
	form_tree_->destroy_syntax_tree_t(form_tree_->root_);
}