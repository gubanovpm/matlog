#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
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

cnf_t::cnf_t(const cnf_t &other) {
	variables_ = other.variables_;
	disjuncts_  = other.disjuncts_;

	int sz = variables_.size();

	val_     = new char [sz] ;
	visited_ = new int  [sz] ;
	balance_ = new int  [sz] ;
	for (int i = 0; i < sz; ++i) {
		val_[i]     = other.val_[i];
		visited_[i] = other.visited_[i];
		balance_[i] = other.balance_[i];
	}
}

cnf_t::cnf_t() {
	val_     = new char [255] ;
	visited_ = new int  [255] ;
	balance_ = new int  [255] ;
}

void create_disjunct_(disjunct_t *disjunct, std::vector <std::string> *variables,  int *visited, int *balance, node_t *c_root) {
	if (c_root == nullptr) return;

	literal_t new_literal;
	switch (c_root->data.k) {
		case NODE_OP : {
			if (c_root->data.u.op == OR) {
				create_disjunct_(disjunct, variables, visited, balance, c_root->left );
				create_disjunct_(disjunct, variables, visited, balance, c_root->right);
			}
			else if (c_root->data.u.op == NOT) {
				new_literal.sign_ = 1;
				std::string name = std::string(c_root->left->data.u.var);
				auto it_cur = std::find(variables->begin(), variables->end(), name);
				new_literal.name_ = std::distance(variables->begin(), it_cur);
				++visited[new_literal.name_];
				if (it_cur == variables->end()) {
					variables->push_back(name);
				}
				disjunct->elem_.insert(new_literal);

			}
			break;
		}

		case NODE_VAR: {
			new_literal.sign_ = 0;
			std::string name = std::string(c_root->data.u.var);
			auto it_cur = std::find(variables->begin(), variables->end(), name);
			new_literal.name_ = std::distance(variables->begin(), it_cur);
			++visited[new_literal.name_];
			++balance[new_literal.name_];
			if (it_cur == variables->end()) 
				variables->push_back(name);
			disjunct->elem_.insert(new_literal);

			break;
		}

		case NODE_VAL: {
			int num;
			if (c_root->data.value == 1) num = -2 ;
				else num = -1;
			new_literal.name_ = num;
			new_literal.sign_ = 0;
			disjunct->elem_.insert(new_literal);
			break;
		}
		default:
			printf("default?\n");
			abort();
	}
}

void create_cnf_(cnf_t *cnf, node_t *c_root) {
	if (c_root == nullptr) return;

	for (int i = 0; i < 255; ++i)
		cnf->val_[i] = -1;

	if (c_root->data.k == NODE_OP && c_root->data.u.op == AND) {
		create_cnf_(cnf, c_root->left );
		create_cnf_(cnf, c_root->right);
	}
	else {
		disjunct_t new_disjunct;
		create_disjunct_(&new_disjunct, &cnf->variables_, cnf->visited_, cnf->balance_, c_root);
		cnf->disjuncts_.push_back(new_disjunct);
	}
}

char disjunct_t::eval_disjunct(char *eval) {
	int prec = 0;
	for (auto it: elem_) {
		if (it.name_ == -2) return 1;
		if (it.name_ == -1) continue;
		if (eval[it.name_] == -1) ++prec;
		if (eval[it.name_] != -1 && (eval[it.name_] ^ it.sign_)) return 1; 
	}
	if (prec != 0) return -1;
	return 0;
}

char cnf_t::eval() {
	int temp ;
	for (auto it: disjuncts_) {
		if (it.elem_.size() == 0) continue;
		
		temp = it.eval_disjunct(val_);
		if (temp == -1) return -1;
		if (temp == 0 ) return  0;
	}
	return 1;
}

void disjunct_t::print() {
	if (elem_.size() == 0) return ;
	for (auto it: elem_) {
		std::cout << it.name_ << " with sign = " << it.sign_ << "; ";
	}
	std::cout << std::endl;
}

void cnf_t::print() {
	
	for (auto it: disjuncts_) {
		it.print();
	}
}

void cnf_t::_3form() {
	int count = 0;
	for (auto state: disjuncts_) {
		state = *(disjuncts_.begin() + count);
		if (state.elem_.size() == 0) continue;
		int temp_state_size = state.elem_.size();
		if (temp_state_size < 3) {
			for (int i = 0; i < 3 - temp_state_size; ++i)
				state.elem_.insert({-1, 0});
		}
		if (temp_state_size > 3) {
			int num_of_var = variables_.size();
			variables_.push_back("_");
			literal_t insert_literal = {num_of_var, 0};
			literal_t new_literal;

			while (temp_state_size > 2) {
				disjunct_t new_disjunct = {};

				variables_.push_back("_");
				new_literal = {num_of_var, 1};
				new_disjunct.elem_.insert(new_literal);

				new_disjunct.elem_.insert({state.elem_.begin()->name_, state.elem_.begin()->sign_});
				state.elem_.erase(state.elem_.begin());
				--temp_state_size;
				++num_of_var;

				variables_.push_back("_");
				new_literal = {num_of_var, 0};
				new_disjunct.elem_.insert(new_literal);

				disjuncts_.push_back(new_disjunct);
			}
			state.elem_.insert(insert_literal);
			disjuncts_.push_back(state);
			disjuncts_[count] = {};
		}
		++count;
	}
}

 
void cnf_t::unit_propagate() {
	bool isPropagate = false;
	literal_t literal = {-3, 0};
	for (auto it:disjuncts_) {
		if (it.elem_.size() == 0) continue;

		int counter = 0;
		if (it.eval_disjunct(val_) == -1)
			for (auto i: it.elem_) {
				if (val_[i.name_] == -1) {
					++counter;
					literal = i;
				}
				if (counter > 1) break;
			}

		if (counter == 1) {
			isPropagate = true;
			val_[literal.name_] = 1 ^ literal.sign_;
		}
	}

	if (isPropagate) unit_propagate();
}

void cnf_t::pure_literal() {
	for (long unsigned int i = 0; i < variables_.size(); ++i) {
		if (visited_[i] == 0) continue;
		if (visited_[i] == balance_[i] || !balance_[i]) {
			if (!balance_[i]) val_[i] = 0;
				else val_[i] = 1;
		}
	}
}

bool DPLL(cnf_t cnf) {
	char isEnd = cnf.eval();
	if (isEnd == 1) return true ;
	if (isEnd == 0) return false;

	cnf.unit_propagate();
	cnf.pure_literal();

	for (auto state: cnf.disjuncts_) {
		if (state.elem_.size() == 0) continue;
		char temp = state.eval_disjunct(cnf.val_);
		if (temp == 1) {
			for (auto it: state.elem_) {
				--cnf.visited_[it.name_];
				if (it.sign_ == 0) --cnf.balance_[it.name_];
			}
			state = {};
		}
		if (temp == 0) return false;
	}

	int new_var = 0;

	for (long unsigned int i = 0 ; i < cnf.variables_.size(); ++i) {
		if (cnf.val_[i] == -1) {
			isEnd   = 0;
			new_var = i;
			break;
		}
	}

	if (!isEnd) return cnf.eval();
	
	cnf.val_[new_var] = 1;
	bool temp_1 = DPLL(cnf);
	if (temp_1) return true;
	
	cnf.val_[new_var] = 0;
	return DPLL(cnf);
}

