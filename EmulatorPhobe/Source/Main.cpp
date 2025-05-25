#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "Bus.h"
#include "Cpu.h"
#include "Lex.h"
#include "Compiler.h"

const std::string helpMsg = R"(
	Actions:
	n, next  - Run the next instruction
	r, reset - Reset
	i, irq   - Interrupt request
	nmi      - Non-maskable interrupt
	h, help  - Display this message
	q, quit  - Quit lol
)";

Bus nes;
std::map<uint16_t, std::string> mapAsm;

std::string hex(uint32_t n, uint8_t d)
{
	std::string s(d, '0');
	for (int i = d - 1; i >= 0; i--, n >>= 4)
		s[i] = "0123456789ABCDEF"[n & 0xF];
	return s;
};

void DrawCpu()
{
	std::cout << "\n\nStatus: "
			  << ((nes.cpu.status & Cpu::N) ? "(N)" : "N") << " "
			  << ((nes.cpu.status & Cpu::V) ? "(V)" : "V") << " "
			  << ((nes.cpu.status & Cpu::U) ? "(-)" : "-") << " "
			  << ((nes.cpu.status & Cpu::B) ? "(B)" : "B") << " "
			  << ((nes.cpu.status & Cpu::D) ? "(D)" : "D") << " "
			  << ((nes.cpu.status & Cpu::I) ? "(I)" : "I") << " "
			  << ((nes.cpu.status & Cpu::Z) ? "(Z)" : "Z") << " "
			  << ((nes.cpu.status & Cpu::C) ? "(C)" : "C") << " " << std::endl;

	std::cout << "\n\nPC: $" + hex(nes.cpu.pc, 4) << "\n"
			  << "A: $" + hex(nes.cpu.a, 2) + "  [" + std::to_string(nes.cpu.a) + "]" << "\n"
			  << "X: $" + hex(nes.cpu.x, 2) + "  [" + std::to_string(nes.cpu.x) + "]" << "\n"
			  << "Y: $" + hex(nes.cpu.y, 2) + "  [" + std::to_string(nes.cpu.y) + "]" << "\n"
			  << "Stack P: $" + hex(nes.cpu.stkp, 4) << "\n\n" << std::endl;
}

void DrawCode(int lines)
{
	int8_t low = nes.ram[0xFFFC];
	int8_t high = nes.ram[0xFFFD];
	int16_t start_addr = (high << 8) | low;

	auto it_a = mapAsm.find(start_addr);
	int count = 1;
	if (it_a != mapAsm.end())
	{
		std::cout << ((mapAsm.find(nes.cpu.pc) == it_a) ? "> " : "  ")
				  << (*it_a).second << std::endl;
		while (count < lines)
		{
			count += 1;
			if (++it_a != mapAsm.end())
			{
				std::cout << ((mapAsm.find(nes.cpu.pc) == it_a) ? "> " : "  ")
						  << (*it_a).second << std::endl;
			}
		}
	}
}

static void handleActions(int instrNum)
{
	static bool printHelp = false;
	DrawCpu();
	DrawCode(instrNum);
	if (printHelp)
	{
		printHelp = false;
		std::cout << helpMsg << std::endl;
	}
	std::string action;
	std::cout << "Enter action: ";
	std::cin >> action;

	/*
	copied this whole shi off of stackoverflow but basically
	it transforms all characters of a string into lowercase
	*/
	std::transform(action.begin(), action.end(), action.begin(),
		[](unsigned char c) { return std::tolower(c); });


	if (action == "q" || action == "quit")
		return;
	else if (action == "h" || action == "help")
		printHelp = true;
	else if (action == "n" || action == "next")
	{
		do
		{
			nes.cpu.clock();
		} while (!nes.cpu.complete());
	}
	else if (action == "r" || action == "reset")
		nes.cpu.reset();
	else if (action == "i" || action == "irq")
		nes.cpu.irq();
	else if (action == "nmi")
		nes.cpu.nmi();
	handleActions(instrNum);
}

static bool run(std::string str)
{
	Lex lex(str);
	std::vector<Token> tokens = lex.lexAll();
	Compiler compiler(tokens);
	if (!compiler.compile())
		return false;
	
	std::vector<uint8_t> bytecodes = compiler.getBytecodes();
	
	std::cout << "Bytecodes: ";
	uint16_t nOffset = 0x8000;
	for (uint8_t byte : bytecodes)
	{
		uint16_t toPrint = byte & 0x00FF; // c++ cant print int8_t in hex :(
		std::cout << std::hex << toPrint << " ";
		nes.ram[nOffset++] = byte;
	}
	std::cout << std::endl;
	
	/* Specify that code starts at 0x8000 for reset */
	nes.ram[0xFFFC] = 0x00;
	nes.ram[0xFFFD] = 0x80;

	// Extract dissassembly
	mapAsm = nes.cpu.disassemble(0x0000, 0xFFFF);

	// Reset
	nes.cpu.reset();
	
	handleActions(lex.getInstrNum());
	return true;
}

static bool runFile(char* file)
{
	std::ifstream fileStream(file);
	std::ostringstream oss;
	oss << fileStream.rdbuf();
	std::string fileStr = oss.str();
	if (fileStr.empty())
		return false;
	return run(fileStr);
}

int main(int argc, char **argv)
{
#ifndef _DEBUG
	if (argc != 2)
	{
		std::cerr << "Usage: " << argv[0] << " [file]" << std::endl;
		return -1;
	}
#else
	std::string src = R"(
			LDX #$0A
			STX $0004
			LDX #$00
			STX $0005 ; store it in reverse cuz lil endian
			LDX #$AA
			STX $000A
			LDX #3
			LDY #5	
			LDA #0
			CLC         ; How that ADC instruction works:
			ADC ($01,x) ; $01 + $03 (x)       =	$0004 (low bit)
			DEY         ; $01 + $03 (x) + $01 = $0005 (high bit)
			BNE $FA     ; read $0004 -> $0A (low bit), read $0005 -> $00 (high bit)
			STA $0002   ; $HHLL = $000A, read $000A -> $AA, add $AA to accumulator
			NOP
			NOP
			NOP
)";
	return run(src);
#endif
	return runFile(argv[1]);
}