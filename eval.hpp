#pragma once

#include <iostream>
#include <unordered_map>
#include <list>

struct eval_t {
	std::unordered_map < std::string, std::list < node_t * > > pointer_table_;
	syntax_tree_t *form_tree_;

	eval_t (syntax_tree_t &copied_tree, std::string arg);
	node_t *parse_eval(node_t *current);
	~eval_t();
};