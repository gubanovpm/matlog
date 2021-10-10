#pragma once

#include <iostream>
#include <unordered_set>
#include <set>

#include "lexer.hpp"
#include "parser.hpp"
#include "eval.hpp"

struct literal_t {
	int  name_;
	bool sign_ = 0;

};

bool operator> (const literal_t &left, const literal_t &right);
bool operator< (const literal_t &left, const literal_t &right);

struct disjunct_t {
	std::set <literal_t> elem_;
	
	bool eval_disjunct(bool *eval);
};

bool operator> (const disjunct_t &left, const disjunct_t &right);
bool operator< (const disjunct_t &left, const disjunct_t &right);

void create_disjunct_(disjunct_t *disjunct, std::unordered_set <std::string> *variables, node_t *c_root);

struct cnf_t {
	std::unordered_set <std::string> variables_;
	std::set <disjunct_t> dijuncts_;
	bool *eval_;


};

void create_cnf_(cnf_t *cnf, node_t *c_root);