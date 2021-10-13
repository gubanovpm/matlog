#pragma once

#include <iostream>
#include <list>
#include <unordered_map>

void inorder_create_list(node_t *current, std::list < std::string > * lst);

static int flag_changed = 0;

struct eval_t {
	syntax_tree_t *form_tree_;

	eval_t(syntax_tree_t &copied_tree);
	syntax_tree_t evaluation(std::string data);

	node_t *translate(node_t *current, std::unordered_map < std::string, bool > variables_table);

	std::list < std::string > *create_variables_list(node_t *root);
	bool is_TAUT();
	bool  is_SAT();

	node_t *parse_eval         (node_t *current);
	node_t *second             (node_t *current);
	syntax_tree_t *cnf_form    (syntax_tree_t *copied_tree);

	node_t *de_morgans_laws    (node_t *current);
	node_t *law_of_distributivity(node_t *current);

	~eval_t();
};