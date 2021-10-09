#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <cstring>

/*
	first argument  = filename
	second argument = max count of variables
	third argument  = count of operation
*/

void itoa(int res, char *buf, int strl);
int sizeofnum(int res);

int main(int argc, char **argv) {
	srand(time(nullptr));

	if (argc != 4) {
		std::cout << "Oops - wrong arguments\n" << std::endl;
		return -1;
	}
	FILE *wrt = fopen(argv[1], "w");
	int x, v, n, br = 0, f_br;
	std::string result = "" ;

	int sz = strlen(argv[2]);
	char *var_num = new char[sz];

	for (int i = 0; i < atoi(argv[3]); ++i) {
		x = rand() % 3;
		v = rand() % atoi(argv[2]);

		itoa(v, var_num, sz); 
		
		n = rand() % 2;
		if (n) result += "~";
		
		f_br = rand() % 2;
		if (f_br) {
			result += "(" ;
			++br;
		}

		n = rand() % 2;
		if (n) result += "~";

		switch (x) {
			case 0: result += ("v" + std::string(var_num) + "|")  ; break;
			case 1: result += ("v" + std::string(var_num) + "&")  ; break;
			case 2: result += ("v" + std::string(var_num) + "->") ; break;
		}
		//std::cout << x << " " << result << std::endl;
	}
	v = rand() % atoi(argv[2]);
	n = rand() % 2;
	if (n) result += "~";
	itoa(v, var_num, sz); 
	result += ("v" + std::string(var_num));

	for (int i = 0; i < br; ++i)
		result += ")" ;
	
	fprintf(wrt, "%s", result.data());
	fclose(wrt);

	return 0;
}

void itoa(int res, char *buf, int strl) {
	//std::cout << res << " -> ";
	for (int i = 0; i < strl; ++i)
		buf[i] = 0;
	for (int i = sizeofnum(res) - 1; i >= 0 ; --i) {
		buf[i] = res % 10 + '0';
		res /= 10;
	}
	//std::cout << buf << std::endl;
}

int sizeofnum(int res) {
	int sz = 0;
	if (res == 0) ++sz;
	while (res != 0) {
		++sz;
		res /= 10;
	}
	return sz;
}