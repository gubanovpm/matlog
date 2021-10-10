#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <set>

#include "lexer.hpp"
#include "parser.hpp"
#include "eval.hpp"
#include "cnf.hpp"

void create_disjunct_(disjunct_t *disjunct, std::set <std::string> *variables, node_t *c_root) {
	if (c_root == nullptr) return;

	literal_t new_literal;
	switch (c_root->data.k) {
		case NODE_OP:
			switch (c_root->data.u.op) {
				case OR:
					create_disjunct_(disjunct, variables, c_root->left );
					create_disjunct_(disjunct, variables, c_root->right);
					return;
				case NOT:
					new_literal.sign_ = 1;
					std::string name = std::string(c_root->left->data.u.var);
					auto it_cur = variables->find(name);
					if (it_cur == variables->end()) 
						variables->insert(name);
					new_literal.name_ = std::distance(it_cur, variables->begin());
					disjunct->elem_.insert(new_literal);
					return;
				default:
					std::cout << "unexpected operation in disjunct" << std::endl;
					abort();
			}
			break;

		case NODE_VAR:
			new_literal.sign_ = 1;
			std::string name = std::string(c_root->data.u.var);
			auto it_cur = variables->find(name);
			if (it_cur == variables->end()) 
				variables->insert(name);
			new_literal.name_ = std::distance(it_cur, variables->begin());
			disjunct->elem_.insert(new_literal);
			return;

		case NODE_VAL:
			int name;
			if (c_root->data->value == 1) name = -1 ;
				else name = -2;
			new_literal.name_ = name;
			new_literal.sign_ = 0;
			disjunct->elem_.insert(new_literal);
			return;

		default:
			std::cout << "unknown node_t type" << std::endl;
			abort();
	}
}

void create_cnf_(cnf_t *cnf, node_t *c_root) {
	if (c_root == nullptr) return;
	
	switch(s_tree->data)
}