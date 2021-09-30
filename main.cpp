#include "parser.hpp"
#include <string>
int main () {

	lex_array_t array("(v1 -> v2 + fghjklytdd) &   v (erty | hg)\0");
	for (int i = 0; i < array.size_; ++i) {
		std::cout << array.lexems_[i] ;
	}
	std::cout << std::endl;
	return 0;
}