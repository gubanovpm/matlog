#pragma once

#include <iostream>
#include <unordered_map>

// description of all required types
enum operation_t  { AND, OR, NOT, XOR, IMPL, EQUAL };
enum braces_t     { LBRAC, RBRAC };
enum lexem_kind_t { OP, BRACE, VAR , TAUT, FALSE};
enum node_kind_t  { NODE_OP, NODE_VAL };

struct lexem_t {
	enum lexem_kind_t kind;
	union {
		enum operation_t op;
		enum braces_t b;
		char *var;
	} lex;
};

struct lex_array_t {
	struct lexem_t *lexems;
	int size;
	int capacity;

	lex_array_t(const char *str);
};

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
//------------------------------------------------------------------------------------------------

lex_array_t::lex_array_t(const char *str) {
	// TODO : your code here
}

