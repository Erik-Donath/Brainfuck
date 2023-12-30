#include <iostream>

#include "Types.h"
#include "Assembler.h"
static void printHelp();

i32 main(i32 argc, cstr argv[]) {
	if (argc <= 1) {
		std::cout << "E: There is no input file given." << std::endl;
		std::cout << "I: To run a BF Programm, run the command below in your terminal and replace FILE.bf with the Filepath.\n   " << argv[0] << " FILE.bf" << std::endl << std::endl;
		printHelp();
		return 1;
	}

	Assembler assembler;
	assembler.assemble(argv[1]);

	return 0;
}

static void printHelp() {
	std::cout << "Here is an short explination of BF:"			  << std::endl << std::endl;
	std::cout << "Instruction\t" << "What it does"				  << std::endl;
	std::cout << "----------------------------------------------" << std::endl;
	std::cout << " +\t\t" << "Increments the current slot"		  << std::endl;
	std::cout << " -\t\t" << "Decrements the current slot"		  << std::endl;
	std::cout << " ,\t\t" << "Input a char into the curent slot"  << std::endl;
	std::cout << " .\t\t" << "Output the curent slot as ASCII"	  << std::endl;
	std::cout << " <\t\t" << "Moves the Slot pointer left"		  << std::endl;
	std::cout << " >\t\t" << "Moves the Slot pointer right"		  << std::endl;
	std::cout << " [\t\t" << "Begining of an Loop"				  << std::endl;
	std::cout << " ]\t\t" << "Ending of an Loop"				  << std::endl;

	std::cout << std::endl << "Every other Charecter is ignored!" << std::endl;
	std::cout << std::endl << "Loops: "							  << std::endl;
	std::cout << "When the Interpreter hits an open breaked it checks if the current slot is 0. If it is the Interpreter skips the Loop and direktly jumps to the coresponding closing breaked. When the Interpreter hits an closing breaked it checks if the current slot is not 0 and then jumps to the coresponding opening breaked. Otherwise it does not jump." << std::endl;
}