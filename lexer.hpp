#pragma once

#include <iostream>

enum braces_t     { LBRAC = 0 , RBRAC };
enum operation_t  { AND   = 11, OR , NOT , IMPL};
enum lexem_kind_t { OP    = 21, BRAC, VAR , T, F};

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
	lexem_t *lexems_;
	int size_;
	int capacity_;

	lex_array_t (const char *str);
	void resize(int new_size);
	~lex_array_t();
};

std::ostream &operator<< (std::ostream &left, lexem_t &lexem) {
	switch (lexem.kind) {
	case OP:
		switch (lexem.lex.op) {
		case OR:
			std::cout << "|" ;
			break;
		case AND:
			std::cout << "&" ;
			break;
		case NOT:
			std::cout << "~" ;
			break;
		case IMPL:
			std::cout << "->" ;
			break;
		default:
			std::cout << "<unknown operation type>";
			break;
		}
		break;

	case BRAC:
		switch (lexem.lex.b) {
			case RBRAC:
				std::cout << ")" ;
				break;
			case LBRAC:
				std::cout << "(";
				break;
			default:
				std::cout << "<unknown bracket type>";
				break;
		}
		break;

	case VAR:
		std::cout << lexem.lex.var ;
		break;
	
	case T:
		std::cout << "1" ;
		break;

	case F:
		std::cout << "0";
		break;
	
	default:
		std::cout << "<unknown lexem type>" ;
		break;
	}
	return left;
}

lex_array_t::lex_array_t(const char *str) {
	
	size_     = 0 ;
	capacity_ = 10;
	lexems_   = new lexem_t [capacity_];

	int counter = 0;
	while (str[counter] != '\0') {

		if (size_ >= capacity_)
			resize(2 * capacity_);

		if (isdigit(str[counter])) {
					std::cout << "Variable names can not starting with a number\n" ;
					abort();
				}

		switch (str[counter]) {
			case '(':
				lexems_[size_].kind  = BRAC ;
				lexems_[size_].lex.b = LBRAC;
				break;

			case ')':
				lexems_[size_].kind  = BRAC;
				lexems_[size_].lex.b = RBRAC;
				break;

			case '~':
				lexems_[size_].kind   = OP;
				lexems_[size_].lex.op = NOT;
				break;

			case '&' :
				lexems_[size_].kind   = OP;
				lexems_[size_].lex.op = AND;
				break;

			case '|' :
				lexems_[size_].kind   = OP;
				lexems_[size_].lex.op = OR;
				break;

			case '-' :
				if (str[++counter] != '>') {
					std::cout << "Syntax error - unknow operation \'-\'!\n" ;
					abort();
				}

				lexems_[size_].kind   = OP;
				lexems_[size_].lex.op = IMPL;
				break;

			case '1' :
				lexems_[size_].kind      = T;
				lexems_[size_].lex.value = 1;
				break;

			case '0' :
				lexems_[size_].kind      = F;
				lexems_[size_].lex.value = 0;
				break;

			default:
				if (isalpha(str[counter])) {
					std::string temp_name = "";
					while (isdigit(str[counter]) || isalpha(str[counter])) {
						temp_name += str[counter];
						++counter;
					}
					--counter;
					lexems_[size_].kind    = VAR;
					lexems_[size_].lex.var = new char [temp_name.length() + 1];
					for (int i = 0; i < temp_name.length(); ++i)
						lexems_[size_].lex.var[i] = temp_name[i];
					lexems_[size_].lex.var[temp_name.length()] = '\0';
					++size_;
				}
				++counter;
				continue;
		}
		++size_;
		++counter;
	}
}

lex_array_t::~lex_array_t() {
	for (int i = 0; i < size_; ++i)
		if (lexems_[i].kind == VAR)
			delete [] lexems_[i].lex.var;
	delete [] lexems_;
}

void lex_array_t::resize(int new_size) {
	lexem_t *new_lexems = new lexem_t [new_size];
	int copy_size = (capacity_ < new_size) ? capacity_ : new_size ;
	for (int i = 0; i < copy_size; ++i) {
		new_lexems[i] = lexems_[i];
	}

	delete [] lexems_;
	lexems_ = new_lexems;
	capacity_ = new_size;
}