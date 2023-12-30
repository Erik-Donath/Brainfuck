#pragma once
#include <list>
#include <vector>

#include "Types.h"

enum Type : u8 {
	NO_INST		 = 0,
	Addition	 = 1,
	Substraction = 2,
	Output		 = 3,
	Input		 = 4,
	Left		 = 5,
	Right		 = 6,
	OpenLoop	 = 7,
	CloseLoop	 = 8
};
struct Instruction {
	Type type;

	union {
		u32 data;
		u32 amound;
		u32 pointer;
	};

	Instruction(Type type)			 : type(type), data(0)	  { }
	Instruction(Type type, u32 data) : type(type), data(data) { }
};
struct Location {
	u32 line	 = 0;
	u32 position = 0;

	inline void incLine() {
		line	+= 1;
		position = 0;
	}
	inline void incPosition() {
		position += 1;
	}
	inline void set(Location next) {
		line	 = next.line;
		position = next.position;
	}
	inline void set(u32 nline, u32 nposition) {
		line	 = nline;
		position = nposition;
	}
};
struct LoopPointer { // Pointer means in this case the position where the instruction is stored. (Not an real memory pointer!)
	u32		 pointer;
	Location location;

	LoopPointer(u32 pointer)					: pointer(pointer)					   { }
	LoopPointer(u32 pointer, Location location) : pointer(pointer), location(location) { }
};
class Assembler {
public:
	 Assembler();
	~Assembler();

	void assemble(cstr filepath);
private:
	Type convert(char c);
	void put(char c, u32 amound);

	std::vector<Instruction>* instructions;
	std::list<LoopPointer>*   loopPointers;

	Location startInstruction;
	Location endInstruction;
};