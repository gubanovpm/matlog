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
		enum braces_t    b;
		char *var;
		int  value;
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
//------------------------------------------------------------------------------------------------

std::ostream &operator<< (std::ostream &left, lexem_t &) {
	switch ()
	{
	case /* constant-expression */:
		/* code */
		break;
	
	default:
		break;
	}
	return left;
}

lex_array_t::lex_array_t(const char *str) {

	//std::cout << "→" << std::endl;
	
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
			case '~':
				lexems[size].kind   = OP;
				lexems[size].lex.op = NOT;
				break;
			case '&' :
				lexems[size].kind   = OP;
				lexems[size].lex.op = AND;
				break;
			case '|' :
				lexems[size].kind   = OP;
				lexems[size].lex.op = OR;
				break;
			case '+' :
				lexems[size].kind   = OP;
				lexems[size].lex.op = XOR;
				break;
			case '-' :
				while (str[++counter] != '\0' && str[counter] != '>');
				if (str[counter] != '>') {
					std::cout << "Syntax error - unknow operation \'-\'!\n" ;
					abort();
				}
				lexems[size].kind   = OP;
				lexems[size].lex.op = IMPL;
				break;
			case '=' :
				lexems[size].kind   = OP;
				lexems[size].lex.op = EQUAL;
				break;
			case '1' :
				lexems[size].kind      = T;
				lexems[size].lex.value = 1;
				break;
			case '0' :
				lexems[size].kind      = F;
				lexems[size].lex.value = 0;
				break;
			default:
				if (isalpha(str[counter])) {
					std::string temp_name = "";
					while (isdigit(str[counter]) || isalpha(str[counter])) {
						temp_name += str[counter];
						++counter;
					}
					lexems[size].kind    = VAR;
					lexems[size].lex.var = new char [temp_name.length() + 1];
					for (int i = 0; i < temp_name.length(); ++i)
						lexems[size].lex.var[i] = temp_name[i];
					lexems[size].lex.var[temp_name.length()] = '\0';
				}
		}
		++size;
		++counter;
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