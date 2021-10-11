#pragma once

enum bracket_t    { LBRAC = 11, RBRAC};
enum operation_t  { AND   = 21, OR   , NOT , IMPL, EQUAL};
enum lexem_kind_t { OP    = 31, BRAC , VAR , T   , F    };

struct lexem_t {
	enum lexem_kind_t kind;
	union {
		enum operation_t op;
		enum bracket_t    b;
		char *var;
		int  value;
	} lex;
};

struct lex_array_t {
	lexem_t *lexems_;  // vector
	int size_;
	int capacity_;

	lex_array_t (const char *str);
	void resize(int new_size);
	~lex_array_t();
};

std::ostream &operator<< (std::ostream &left, lexem_t &lexem);