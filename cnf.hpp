#pragma once

#include <unordered_map>
#include <iostream>
#include <set>

#include "lexer.hpp"
#include "parser.hpp"
#include "eval.hpp"

struct literal_t {
	int  name_;
	bool sign_ = 0;
};

struct disjunct_t {
	std::set <literal_t> elem_;
	
	bool eval_disjunct(bool *eval);
};

void create_disjunct_(disjunct_t *disjunct, std::set <std::string> *variables, node_t *c_root);

struct cnf_t {
	std::set <std::string> variables_;
	std::set <disjunct_t>   dijuncts_ ;
	bool                   *eval_    ;


};

void create_cnf_(cnf_t *cnf, node_t *c_root);