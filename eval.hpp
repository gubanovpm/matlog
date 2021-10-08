#pragma once

#include <iostream>
#include <unordered_map>

struct eval_t {
	syntax_tree_t *form_tree_;

	eval_t (syntax_tree_t &copied_tree);
	syntax_tree_t evaluation(std::string data);

	node_t *translate(node_t *current, std::unordered_map < std::string, bool > variables_table);

	bool is_TAUT();
	bool  is_SAT();

	// TODO: create unordered_map with all used variables
	// TODO: write is_TAUT and is_SAT with exp commplexity
	// TODO: last step to cnf form(in tree view) - disjunctions and absorbtion laws

	node_t *parse_eval         (node_t *current);
	node_t *second             (node_t *current);
	node_t *de_morgans_laws    (node_t *current);
	
	~eval_t();
};