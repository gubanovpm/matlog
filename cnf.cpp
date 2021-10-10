#include <algorithm>
#include <string>
#include <iostream>
#include <set>

#include "lexer.hpp"
#include "parser.hpp"
#include "eval.hpp"
#include "cnf.hpp"

bool operator> (const literal_t &left, const literal_t &right) {
	return (left.name_ > right.name_);
}
bool operator< (const literal_t &left, const literal_t &right) {
	return (left.name_ < right.name_);
}

bool operator> (const disjunct_t &left, const disjunct_t &right){
	return (left.elem_.size() > right.elem_.size());
}
bool operator< (const disjunct_t &left, const disjunct_t &right){
	return (left.elem_.size() < right.elem_.size());
}

void create_disjunct_(disjunct_t *disjunct, std::unordered_set <std::string> *variables, node_t *c_root) {
	if (c_root == nullptr) return;

	literal_t new_literal;
	switch (c_root->data.k) {
		case NODE_OP : {
			if (c_root->data.u.op == OR) {
				create_disjunct_(disjunct, variables, c_root->left );
				create_disjunct_(disjunct, variables, c_root->right);
			}
			else if (c_root->data.u.op == NOT) {
				new_literal.sign_ = 1;
				std::string name = std::string(c_root->left->data.u.var);
				auto it_cur = variables->find(name);
				new_literal.name_ = std::distance(variables->begin(), it_cur);
				if (it_cur == variables->end()) {
					variables->insert(name);
				}
				disjunct->elem_.insert(new_literal);
			}
			break;
		}

		case NODE_VAR: {
			new_literal.sign_ = 0;
			std::string name = std::string(c_root->data.u.var);
			auto it_cur = variables->find(name);
			new_literal.name_ = std::distance(variables->begin(), it_cur);
			if (it_cur == variables->end()) 
				variables->insert(name);
			disjunct->elem_.insert(new_literal);
			break;
		}

		case NODE_VAL: {
			int num;
			if (c_root->data.value == 1) num = -1 ;
				else num = -2;
			new_literal.name_ = num;
			new_literal.sign_ = 0;
			disjunct->elem_.insert(new_literal);
			break;
		}
		default:
			printf("Why are you gay?\n");
			abort();
	}
}

void create_cnf_(cnf_t *cnf, node_t *c_root) {
	if (c_root == nullptr) return;

	if (c_root->data.k && c_root->data.u.op == AND) {
		create_cnf_(cnf, c_root->left );
		create_cnf_(cnf, c_root->right);
	}
	else {
		disjunct_t new_disjunct;
		create_disjunct_(&new_disjunct, &cnf->variables_, c_root);
		cnf->dijuncts_.insert(new_disjunct);
	}
}
/*
bool disjunct_t::eval_disjunct(bool *eval) {
	for (auto it: elem_) {

	}
}*/

void disjunct_t::print() {
	for (auto it: elem_) {
		std::cout << it.name_ << " with sign = " << it.sign_ << "; ";
	}
	std::cout << std::endl;
}

void cnf_t::print() {
	
	for (auto it: dijuncts_) {
		it.print();
	}
}