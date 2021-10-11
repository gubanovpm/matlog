#include <iostream>
#include "lexer.hpp"

std::ostream &operator<< (std::ostream &left, lexem_t &lexem) {
	switch (lexem.kind) {
	case OP:
		switch (lexem.lex.op) {
			case OR:    std::cout << "|"  ; break;
			case AND:   std::cout << "&"  ; break;
			case NOT:   std::cout << "~"  ; break;
			case IMPL:  std::cout << "->" ; break;
			case EQUAL:	std::cout << "="  ; break;
			default:    std::cout << "<unknown operation type>"; exit(-1) ; break;
		}
		break;

		case BRAC:
			switch (lexem.lex.b) {
				case RBRAC: std::cout << ")" ; break;
				case LBRAC: std::cout << "(" ; break;
				default: std::cout << "<unknown bracket type>"; exit(-1) ; break;
			}
			break;

		case VAR: std::cout << lexem.lex.var ; break;
		case T: std::cout << "1" ; break;
		case F: std::cout << "0" ; break;
		default: std::cout << "<unknown lexem type>" ; exit(-1) ; break;
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
				if (counter < capacity_  && str[counter + 1] != '>') {
					std::cout << "Syntax error - unknow operation \'-\'!\n" ;
					for (int i = 0; i < size_; ++i)
					if (lexems_[i].kind == VAR)
						delete [] lexems_[i].lex.var;
					delete [] lexems_;
					lexems_ = nullptr;
				}

				lexems_[size_].kind   = OP;
				lexems_[size_].lex.op = IMPL;
				counter += 2;
				++size_;
				continue;
				break;

			case '1' :
				lexems_[size_].kind      = T;
				lexems_[size_].lex.value = 1;
				break;

			case '0' :
				lexems_[size_].kind      = F;
				lexems_[size_].lex.value = 0;
				break;

			case '=' :
				lexems_[size_].kind      = OP;
				lexems_[size_].lex.op    = EQUAL;
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
					for (unsigned i = 0; i < temp_name.length(); ++i)
						lexems_[size_].lex.var[i] = temp_name[i];
					lexems_[size_].lex.var[temp_name.length()] = '\0';
					++size_;
				}
				
				else if (isspace (str[counter]) == 0) {
					std::cout << "Unexpected element : " << str[counter] << std::endl;
					for (int i = 0; i < size_; ++i)
					if (lexems_[i].kind == VAR)
						delete [] lexems_[i].lex.var;
					delete [] lexems_;
					lexems_ = nullptr;
					return ;
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