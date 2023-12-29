#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <cstdint>
typedef int8_t   i8;
typedef uint8_t  u8;
typedef int32_t  i32;
typedef uint32_t u32;

enum InstructionType :  u8 {
	NotDefined = 0,
	Increment  = 1,
	Decrement  = 2,
	Left       = 3,
	Right      = 4,
	Print      = 5,
	Input      = 6,
	LoopStart  = 7,
	LoopEnd    = 8
};

const char* InstructionTypeTable[9]{
	"NotDefined",
	"Increment",
	"Decrement",
	"Left",
	"Right",
	"Print",
	"Input",
	"LoopStart",
	"LoopEnd"
};

struct Instruction {
	InstructionType type = NotDefined;

	union {
		u32 amound;
		u32 data;
		u32 pointer;
	};

	Instruction(InstructionType type)           : type(type), data(0)    { }
	Instruction(InstructionType type, u32 data) : type(type), data(data) { }
};

std::vector<Instruction> instructions;
std::list<u8> loopPointers; // Should be at the end of the compilation empty
bool errorInLoop = false;

void printTable() {
	std::cout << "Index\tInstruction\tData" << std::endl;
	std::cout << "-----------------------------" << std::endl;
	for (u32 i = 0; i < instructions.size(); i++) {
		Instruction& e = instructions[i];
		std::cout << i << "\t" << InstructionTypeTable[e.type] << "    \t" << e.data << std::endl;
	}
}

inline void addBasic(InstructionType type, u32 data) {
	instructions.push_back(Instruction(type, data));
}
inline void addLoopStart(u32 amound) {
	for (u32 i = 0; i < amound; i++) {
		loopPointers.push_back(instructions.size());
		addBasic(LoopStart, -1); // Pointer will be set, when the close breaked was hit.
	}
}
inline void addLoopEnd(u32 amound)   {
	for (u32 i = 0; i < amound; i++) {
		if (loopPointers.size() == 0) {
			errorInLoop = true;
			addBasic(LoopEnd, -1);
			return;
		}
		u32 p = loopPointers.back();
		loopPointers.pop_back();

		instructions[p].pointer = instructions.size();
		addBasic(LoopEnd, p);
	}
}

static void put(char c, u32 amound) {
	if (c == 0) return;
	
	switch (c) {
	case '+':
		addBasic(Increment, amound);
		break;
	case '-':
		addBasic(Decrement, amound);
		break;
	case '<':
		addBasic(Left, amound);
		break;
	case '>':
		addBasic(Right, amound);
		break;
	case '.':
		addBasic(Print, amound);
		break;
	case ',':
		addBasic(Input, amound);
		break;
	case '[':
		addLoopStart(amound);
		break;
	case ']':
		addLoopEnd(amound);
		break;
	default:
		break;
	}
}

i32 main(i32 argc, char* argv[]) {
	if (argc == 1) {
		std::cout << "E: There was no input file given." << std::endl;
		std::cout << "I: To run a BF Programm, run the command below in your terminal and replace FILE.bf with the Filepath.\n\t" << argv[0] << " FILE.bf" << std::endl;
		return 1;
	}

	std::fstream file(argv[1]);
	if (!file.is_open()) {
		std::cout << "E: The given input file does not exsist or can't be read by this program." << std::endl;
		return 1;
	}
	std::cout << "I: Now reading the given File (\"" << argv[1] << "\")." << std::endl;

	char c        = '\0';
	char lastChar = '\0';
	u32 amound    = 1;

	while (file.get(c) && c != EOF) {
		if (c != lastChar) {
			put(lastChar, amound);
			amound = 0, lastChar = c;
		}
		lastChar = c;
		amound += 1;
	}
	put(lastChar, amound);
	file.close();

	if (errorInLoop || loopPointers.size() != 0) {
		std::cout << "E: One or more Loops aren't clossed or opened correctly." << std::endl;
		return 1;
	}
	std::cout << "I: Programm has ben compiled into Assembly Code." << std::endl;

	printTable();
	return 0;
}
