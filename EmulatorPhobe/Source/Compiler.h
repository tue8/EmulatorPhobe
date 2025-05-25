#pragma once
#include "Lex.h"

class Compiler
{
public:
	Compiler(std::vector<Token>& tokens);
	bool compile();
	std::vector<uint8_t> getBytecodes();
private:
	std::vector<Token>& tokens;
	std::vector<uint8_t> bytecodes;
	int current;

	bool isAtEnd();
	Token advance();
	bool match(enum TokenType type);
	Token peek();
	bool lineEnded();

	bool instruction(std::string instr);
	bool operand16bit(std::string instr, uint16_t num);
	bool operand8bit(std::string instr, uint16_t num);
	bool operandNone(std::string instr);
	bool operandImm(std::string instr);
	bool operandPreIndexedInd(std::string instr, uint16_t num);
	bool operandPostIndexedInd(std::string instr, uint16_t num);
	bool operandIndexedInd(std::string instr, uint16_t num);
	bool operandInd(std::string instr);
};

