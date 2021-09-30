#pragma once

#include "lexer.hpp"

enum node_kind_t  { NODE_OP = 30, NODE_VAL };

struct node_data_t {
	enum node_kind_t k;
	union {
		enum operation_t op;
		char *var;
	} u;
	
};

struct node_t {
	struct node_t *left, *right;
	struct node_data_t data;
};