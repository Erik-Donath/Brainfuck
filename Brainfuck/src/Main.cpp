#include <iostream>
#include <fstream>

#include <cstdint>
typedef int32_t  i32;
typedef uint32_t u32;

i32 main(i32 argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "There is no input file given." << std::endl;
		return 1;
	}

	std::fstream file(argv[1]);
	if (!file.is_open()) {
		std::cout << "The given input file does not exsist or can't be ready by this program." << std::endl;
		return 1;
	}

	char c = '\0';
	while (file.get(c)) {
		std::cout << c;
	}

	file.close();
}
