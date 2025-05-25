#include "Compiler.h"
#include <iostream>

// 300+ lines of if-statements and tragedies

struct CompilerInstruction
{
    std::string instruction;
    std::string addressingMode;
    friend bool operator==(const CompilerInstruction& lhs, const CompilerInstruction& rhs)
    {
        return (lhs.instruction == rhs.instruction) &&
               (lhs.addressingMode == rhs.addressingMode);
    }
};

std::vector<CompilerInstruction> instructions =
{
    { "BRK", "IMM"},{"ORA", "IZX"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "ORA", "ZP0"},{ "ASL", "ZP0"},{ "???", "IMP"},{ "PHP", "IMP"},{ "ORA", "IMM"},{ "ASL", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "ORA", "ABS"},{ "ASL", "ABS"},{ "???", "IMP"},
    { "BPL", "REL"},{"ORA", "IZY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "ORA", "ZPX"},{ "ASL", "ZPX"},{ "???", "IMP"},{ "CLC", "IMP"},{ "ORA", "ABY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "ORA", "ABX"},{ "ASL", "ABX"},{ "???", "IMP"},
    { "JSR", "ABS"},{"AND", "IZX"},{ "???", "IMP"},{ "???", "IMP"},{ "BIT", "ZP0"},{ "AND", "ZP0"},{ "ROL", "ZP0"},{ "???", "IMP"},{ "PLP", "IMP"},{ "AND", "IMM"},{ "ROL", "IMP"},{ "???", "IMP"},{ "BIT", "ABS"},{ "AND", "ABS"},{ "ROL", "ABS"},{ "???", "IMP"},
    { "BMI", "REL"},{"AND", "IZY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "AND", "ZPX"},{ "ROL", "ZPX"},{ "???", "IMP"},{ "SEC", "IMP"},{ "AND", "ABY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "AND", "ABX"},{ "ROL", "ABX"},{ "???", "IMP"},
    { "RTI", "IMP"},{"EOR", "IZX"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "EOR", "ZP0"},{ "LSR", "ZP0"},{ "???", "IMP"},{ "PHA", "IMP"},{ "EOR", "IMM"},{ "LSR", "IMP"},{ "???", "IMP"},{ "JMP", "ABS"},{ "EOR", "ABS"},{ "LSR", "ABS"},{ "???", "IMP"},
    { "BVC", "REL"},{"EOR", "IZY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "EOR", "ZPX"},{ "LSR", "ZPX"},{ "???", "IMP"},{ "CLI", "IMP"},{ "EOR", "ABY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "EOR", "ABX"},{ "LSR", "ABX"},{ "???", "IMP"},
    { "RTS", "IMP"},{"ADC", "IZX"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "ADC", "ZP0"},{ "ROR", "ZP0"},{ "???", "IMP"},{ "PLA", "IMP"},{ "ADC", "IMM"},{ "ROR", "IMP"},{ "???", "IMP"},{ "JMP", "IND"},{ "ADC", "ABS"},{ "ROR", "ABS"},{ "???", "IMP"},
    { "BVS", "REL"},{"ADC", "IZY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "ADC", "ZPX"},{ "ROR", "ZPX"},{ "???", "IMP"},{ "SEI", "IMP"},{ "ADC", "ABY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "ADC", "ABX"},{ "ROR", "ABX"},{ "???", "IMP"},
    { "???", "IMP"},{"STA", "IZX"},{ "???", "IMP"},{ "???", "IMP"},{ "STY", "ZP0"},{ "STA", "ZP0"},{ "STX", "ZP0"},{ "???", "IMP"},{ "DEY", "IMP"},{ "???", "IMP"},{ "TXA", "IMP"},{ "???", "IMP"},{ "STY", "ABS"},{ "STA", "ABS"},{ "STX", "ABS"},{ "???", "IMP"},
    { "BCC", "REL"},{"STA", "IZY"},{ "???", "IMP"},{ "???", "IMP"},{ "STY", "ZPX"},{ "STA", "ZPX"},{ "STX", "ZPY"},{ "???", "IMP"},{ "TYA", "IMP"},{ "STA", "ABY"},{ "TXS", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "STA", "ABX"},{ "???", "IMP"},{ "???", "IMP"},
    { "LDY", "IMM"},{"LDA", "IZX"},{ "LDX", "IMM"},{ "???", "IMP"},{ "LDY", "ZP0"},{ "LDA", "ZP0"},{ "LDX", "ZP0"},{ "???", "IMP"},{ "TAY", "IMP"},{ "LDA", "IMM"},{ "TAX", "IMP"},{ "???", "IMP"},{ "LDY", "ABS"},{ "LDA", "ABS"},{ "LDX", "ABS"},{ "???", "IMP"},
    { "BCS", "REL"},{"LDA", "IZY"},{ "???", "IMP"},{ "???", "IMP"},{ "LDY", "ZPX"},{ "LDA", "ZPX"},{ "LDX", "ZPY"},{ "???", "IMP"},{ "CLV", "IMP"},{ "LDA", "ABY"},{ "TSX", "IMP"},{ "???", "IMP"},{ "LDY", "ABX"},{ "LDA", "ABX"},{ "LDX", "ABY"},{ "???", "IMP"},
    { "CPY", "IMM"},{"CMP", "IZX"},{ "???", "IMP"},{ "???", "IMP"},{ "CPY", "ZP0"},{ "CMP", "ZP0"},{ "DEC", "ZP0"},{ "???", "IMP"},{ "INY", "IMP"},{ "CMP", "IMM"},{ "DEX", "IMP"},{ "???", "IMP"},{ "CPY", "ABS"},{ "CMP", "ABS"},{ "DEC", "ABS"},{ "???", "IMP"},
    { "BNE", "REL"},{"CMP", "IZY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "CMP", "ZPX"},{ "DEC", "ZPX"},{ "???", "IMP"},{ "CLD", "IMP"},{ "CMP", "ABY"},{ "NOP", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "CMP", "ABX"},{ "DEC", "ABX"},{ "???", "IMP"},
    { "CPX", "IMM"},{"SBC", "IZX"},{ "???", "IMP"},{ "???", "IMP"},{ "CPX", "ZP0"},{ "SBC", "ZP0"},{ "INC", "ZP0"},{ "???", "IMP"},{ "INX", "IMP"},{ "SBC", "IMM"},{ "NOP", "IMP"},{ "???", "IMP"},{ "CPX", "ABS"},{ "SBC", "ABS"},{ "INC", "ABS"},{ "???", "IMP"},
    { "BEQ", "REL"},{"SBC", "IZY"},{ "???", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "SBC", "ZPX"},{ "INC", "ZPX"},{ "???", "IMP"},{ "SED", "IMP"},{ "SBC", "ABY"},{ "NOP", "IMP"},{ "???", "IMP"},{ "???", "IMP"},{ "SBC", "ABX"},{ "INC", "ABX"},{ "???", "IMP"},
};


Compiler::Compiler(std::vector<Token>& tokens) : current(0), tokens(tokens)
{
}

bool Compiler::isAtEnd()
{
	return current >= tokens.size() - 1;
}

Token Compiler::advance()
{
	return tokens.at(current++);
}

bool Compiler::match(enum TokenType type)
{
    if (isAtEnd())
        return false;
    if (tokens.at(current).type != type)
        return false;
    current++;
    return true;
}

Token Compiler::peek()
{
	return tokens.at(current);
}

bool Compiler::lineEnded()
{
    if (!match(TOKEN_NEWLINE) && !isAtEnd())
    {
        std::cerr << "Expected new line cuh..." << std::endl;
        return false;
    }
    return true;
}

// questionable performance
static bool findOpCode(std::string instr, std::string addrMode, uint8_t& opcode)
{
    CompilerInstruction cInstruction = { instr, addrMode };
    auto iterator = std::find(instructions.cbegin(), instructions.cend(), cInstruction);
    if (iterator != instructions.cend())
    {
        opcode = std::distance(instructions.cbegin(), iterator);
        return true;
    }
    return false;
}

bool Compiler::operand16bit(std::string instr, uint16_t num)
{
    uint8_t opcode = 0;
    std::string addrMode = "ABS";

    if  (match(TOKEN_X))     addrMode = "ABX";
    else if (match(TOKEN_Y)) addrMode = "ABY";

    if (findOpCode(instr, addrMode, opcode))
    {
        uint8_t high = (num & 0xFF00) >> 8;
        uint8_t low = (num & 0x00FF);
        bytecodes.push_back(opcode);
        bytecodes.push_back(low);
        bytecodes.push_back(high);
    }
    else
    {
        std::cerr << "error: invalid instruction / operand" << std::endl;
        return false;
    }
    return lineEnded();
}

bool Compiler::operand8bit(std::string instr, uint16_t num)
{
    uint8_t opcode = 0;
    std::string addrMode = "ZP0";

    if (match(TOKEN_X))      addrMode = "ZPX";
    else if (match(TOKEN_Y)) addrMode = "ZPY";

    /* relative addr mode is exclusive to branching instruction */
    if (instr == "BPL" || instr == "BMI" || instr == "BVC" ||
        instr == "BVS" || instr == "BCC" || instr == "BCS" ||
        instr == "BNE" || instr == "BEQ")
    {
        if (addrMode != "ZP0")
        {
            std::cerr << ",x and ,y aint supposed to be here gang" << std::endl;
            return false;
        }
        addrMode = "REL";
    }

    if (findOpCode(instr, addrMode, opcode))
    {
        uint8_t byte = (num & 0x00FF);
        bytecodes.push_back(opcode);
        bytecodes.push_back(byte);
    }
    else
    {
        std::cerr << "error: invalid instruction / operand" << std::endl;
        return false;
    }
    return lineEnded();
}

bool Compiler::operandNone(std::string instr)
{
    uint8_t opcode = 0;
    if (findOpCode(instr, "IMP", opcode))
        bytecodes.push_back(opcode);
    else
    {
        std::cerr << "error: invalid instruction / operand" << std::endl;
        return false;
    }
    return true;
}

bool Compiler::operandImm(std::string instr)
{
    Token numToken = advance();
    if (numToken.type == TOKEN_8BIT)
    {
        uint8_t opcode = 0;
        if (findOpCode(instr, "IMM", opcode))
        {
            uint8_t byte = (numToken.num & 0x00FF);
            bytecodes.push_back(opcode);
            bytecodes.push_back(byte);
        }
        else
        {
            std::cerr << "error: invalid instruction / operand" << std::endl;
            return false;
        }
    }
    else
    {
        std::cerr << "Expected an 8-bit number after #" << std::endl;
        return false;
    }
    return lineEnded();
}

bool Compiler::operandPreIndexedInd(std::string instr, uint16_t num)
{
    uint8_t opcode = 0;
    if (findOpCode(instr, "IZX", opcode))
    {
        uint8_t byte = (num & 0x00FF);
        bytecodes.push_back(opcode);
        bytecodes.push_back(byte);
    }
    else
    {
        std::cerr << "error: invalid instruction / operand" << std::endl;
        return false;
    }

    if (!match(TOKEN_CLOSE_PAREN))
    {
        std::cerr << "Expected ')' after ,x" << std::endl;
        return false;
    }
    return true;
}

bool Compiler::operandPostIndexedInd(std::string instr, uint16_t num)
{
    if (!match(TOKEN_Y))
    {
        std::cerr << "Expected ',y' after ')'" << std::endl;
        return false;
    }

    uint8_t opcode = 0;
    if (findOpCode(instr, "IZY", opcode))
    {
        uint8_t byte = (num & 0x00FF);
        bytecodes.push_back(opcode);
        bytecodes.push_back(byte);
    }
    else
    {
        std::cerr << "error: invalid instruction / operand" << std::endl;
        return false;
    }
    return true;
}

bool Compiler::operandIndexedInd(std::string instr, uint16_t num)
{
    if (match(TOKEN_X))                 return operandPreIndexedInd(instr, num);
    else if (match(TOKEN_CLOSE_PAREN))  return operandPostIndexedInd(instr, num);
    std::cerr << "the fuck did you put inside the parentheses" << std::endl;
    return false;
}

bool Compiler::operandInd(std::string instr)
{
    Token numToken = advance();
    if (numToken.type == TOKEN_16BIT) // exclusive to jmp
    {
        if (instr == "JMP")
        {
            uint8_t high = (numToken.num & 0xFF00) >> 8;
            uint8_t low = (numToken.num & 0x00FF);
            bytecodes.push_back(0x6c);
            bytecodes.push_back(low);
            bytecodes.push_back(high);
        }
        else
        {
            std::cerr << "error: invalid instruction / operand" << std::endl;
            return false;
        }

        advance();

        if (!match(TOKEN_CLOSE_PAREN))
        {
            std::cerr << "Expected ')'" << std::endl;
            return false;
        }
    }
    else if (numToken.type == TOKEN_8BIT)
        return operandIndexedInd(instr, numToken.num);
    else
        std::cerr << "error: expected a number after '('" << std::endl;
    return lineEnded();
}

bool Compiler::instruction(std::string instr)
{
    Token operand = advance();

    switch (operand.type)
    {
    case TOKEN_NEWLINE:     return operandNone(instr); // impl
    case TOKEN_IMM:         return operandImm(instr);
    case TOKEN_OPEN_PAREN:  return operandInd(instr);
    case TOKEN_16BIT:       return operand16bit(instr, operand.num); // abs
    case TOKEN_8BIT:        return operand8bit(instr, operand.num); // rel / zp0
    default:
        std::cerr << "error: invalid operand" << std::endl;
        return false;
    }
}

bool Compiler::compile()
{
    while (!isAtEnd())
    {
        Token token = advance();
        if (token.type == TOKEN_NEWLINE)
            continue;
        else if (token.type == TOKEN_INSTR)
        {
            if (!instruction(token.string))
            {
                std::cout << "error at line:" << token.line << std::endl;
                return false;
            }
        }
        else
        {
            std::cerr << "Expected an instruction." << std::endl;
            return false;
        } 
    }

    return true;
}

std::vector<uint8_t> Compiler::getBytecodes()
{
    return bytecodes;
}