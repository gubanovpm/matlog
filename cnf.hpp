#pragma once

#include <iostream>
#include <vector>
#include <set>

#include "lexer.hpp"
#include "parser.hpp"
#include "eval.hpp"

struct literal_t {
	int  name_;
	bool sign_   =  0;
};

bool operator> (const literal_t &left, const literal_t &right);
bool operator< (const literal_t &left, const literal_t &right);

struct disjunct_t {
	std::set <literal_t> elem_ = {};
	
	void print();
	char eval_disjunct(char *eval, int *visited);
};

bool operator> (const disjunct_t &left, const disjunct_t &right);
bool operator< (const disjunct_t &left, const disjunct_t &right);

void create_disjunct_(disjunct_t *disjunct, std::vector <std::string> *variables, int *visited, int *balance, node_t *c_root);

struct cnf_t {

	cnf_t();
	cnf_t(const cnf_t &other);

	std::vector <std::string> variables_ = {};
	std::set <disjunct_t> disjuncts_ = {};

	char *val_;
	int  *visited_;
	int  *balance_;

	void print();
	char eval();
	
	void unit_propagate();

	void pure_literal();

};

void create_cnf_(cnf_t *cnf, node_t *c_root);
bool DPLL(cnf_t cnf);