#pragma once

#include <iostream>
#include <unordered_map>

// description of all required types
enum operation_t  { AND, OR, NOT, XOR, IMPL, EQUAL };
enum braces_t     { LBRAC, RBRAC };
enum lexem_kind_t { OP, BRAC, VAR , T, F};
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

	lex_array_t (const char *str);
	void resize(int new_size);
	~lex_array_t();
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
	
	size     = 0 ;
	capacity = 10;
	lexems   = new lexem_t [capacity];

	int counter = 0;
	while (str[counter] != '\0') {
		if (size >= capacity) {
			resize(2 * capacity);
			capacity *= 2;
		}
		switch (str[counter]) {
			case '(':
				lexems[size].kind  = BRAC ;
				lexems[size].lex.b = LBRAC;
				break;

			case ')':
				lexems[size].kind  = BRAC;
				lexems[size].lex.b = RBRAC;
				break;

			
		}
		size++;
	}
}

lex_array_t::~lex_array_t() {
	delete [] lexems;
}

void lex_array_t::resize(int new_size) {
	lexem_t *new_lexems = new lexem_t [new_size];
	int copy_size = (capacity < new_size) ? capacity : new_size ;
	for (int i = 0; i < copy_size; ++i) {
		new_lexems[i] = lexems[i];
	}

	delete [] lexems;
	lexems = new_lexems;
}