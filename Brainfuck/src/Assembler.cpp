#include <string>
#include <fstream>
#include <iostream>

#include "Assembler.h"

cstr TypeNameTable[9]{
	"NO_INST",
	"Addition",
	"Substraction",
	"Output",
	"Input",
	"Left",
	"Right",
	"OpenLoop",
	"CloseLoop"
};
#define TypeAt(type, location) TypeNameTable[type] << " on (" << location.line << ':' << location.position << "): "

Assembler::Assembler() {
	instructions = new std::vector<Instruction>();
	loopPointers = new std::list<LoopPointer>();
}
Assembler::~Assembler() {
	delete instructions;
	delete loopPointers;
}
void Assembler::assemble(cstr filepath) {
	std::fstream file(filepath);
	if (!file.is_open() || !file.good()) {
		std::cout << "E: The file \"" << filepath << "\" doesn't exsist or can't be openend by this programm." << std::endl;
		return;
	}

	instructions->clear();
	loopPointers->clear();
	startInstruction.set(0, 0);
	endInstruction.set(startInstruction);

	u32 amound = 0;
	std::string line;
	char lastChar = '\0';
    while (std::getline(file, line) && file.good()) {
		endInstruction.incLine();
		for (char c : line) {
			endInstruction.incPosition();
			amound += 1;
			if (c != lastChar && lastChar != '\0') {
				put(lastChar, amound);
				amound = 0;
				startInstruction.set(endInstruction);
			}
			lastChar = c;
		}
	}
	put(lastChar, amound);
	file.close();

	for (const LoopPointer& ptr : *loopPointers) {
		std::cout << "E: " << TypeAt(Type::OpenLoop, ptr.location) << "There is no closing Breaked." << std::endl
				  << "I: You always have to close a breaked after you opened one." << std::endl;
	}
}
Type Assembler::convert(char c) {
	switch (c) {
	case '+': return Type::Addition;
	case '-': return Type::Substraction;
	case ',': return Type::Input;
	case '.': return Type::Output;
	case '<': return Type::Left;
	case '>': return Type::Right;
	case '[': return Type::OpenLoop;
	case ']': return Type::CloseLoop;
	default:  return Type::NO_INST;
	}
}
void Assembler::put(char c, u32 amound) {
	Type type = convert(c);

	// NO_INST, Addition, Subtraction, Input, Output, Left and Right
	if (type == Type::NO_INST)
		return;
	if (type <= 6)
		instructions->push_back(Instruction(type, amound));
	if (type <= 2 && amound >= 256) {
		std::cout << "W: " << TypeAt(type, startInstruction)
				  << "There is no need to add more than 255 to an slot because it raps around to 0."
				  << std::endl << "I: In this case you can just typ "
				  << (amound % 256) << " instead of " << amound << '.'
				  << std::endl;
	}

	// OpenLoop and CloseLoop
	if (type == Type::OpenLoop) {
		for (u32 i = 0; i < amound; i++) {
			loopPointers->push_back(LoopPointer(instructions->size(), startInstruction)); // TODO: Need to be improved some day
			instructions->push_back(Instruction(Type::OpenLoop, -1));
		}
	}
	if (type == Type::CloseLoop) {
		for (u32 i = 0; i < amound; i++) {
			if (loopPointers->size() == 0) {
				instructions->push_back(Instruction(Type::CloseLoop, -1));
				std::cout << "E: " << TypeAt(Type::CloseLoop, startInstruction)
					<< "There is no opening Breaked!" << std::endl
					<< "I: You always have to open a breaked first before you can close one." << std::endl;
				return;
			}
			u32 ptr = loopPointers->back().pointer;
			loopPointers->pop_back();

			instructions->at(ptr).pointer = instructions->size();
			instructions->push_back(Instruction(Type::CloseLoop, ptr));
		}
	}
}
