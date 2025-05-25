#include "Lex.h"
#include <iostream>
#include <algorithm>

const std::string instructions[] = {
    "ADC", "AND", "ASL", "BCC", "BCS", "BEQ", "BIT", "BMI", "BNE", "BPL",
    "BRK", "BVC", "BVS", "CLC", "CLD", "CLI", "CLV", "CMP", "CPX", "CPY",
    "DEC", "DEX", "DEY", "EOR", "INC", "INX", "INY", "JMP", "JSR", "LDA",
    "LDX", "LDY", "LSR", "NOP", "ORA", "PHA", "PHP", "PLA", "PLP", "ROL",
    "ROR", "RTI", "RTS", "SBC", "SEC", "SED", "SEI", "STA", "STX", "STY",
    "TAX", "TAY", "TSX", "TXA", "TXS", "TYA",
};

Lex::Lex(std::string text) : 
    text(text), start(0), current(0), line(0), instrNum(0)
{
}
  
bool Lex::isAtEnd()
{
    return current >= text.length();
}

char Lex::advance()
{
    return text.at(current++);
}

bool Lex::match(char c)
{
    if (isAtEnd())
        return false;
    if (text.at(current) != c)
        return false;
    current++;
    return true;
}

char Lex::peek()
{
    if (isAtEnd())
        return '\0';
    return text.at(current);
}

char Lex::peekNext()
{
    if (current + 1 >= text.length())
        return '\0';
    return text.at(current + 1);
}

void Lex::addTokenInstr(std::string string)
{
    Token tok = { TOKEN_INSTR, line, string, 0};
    tokens.push_back(tok);
    instrNum += 1;
}

void Lex::addTokenNum(uint16_t num, std::string str)
{
    enum TokenType type = (str.length() > 2) ? TOKEN_16BIT : TOKEN_8BIT;
    Token tok = { type, line, str, num};
    tokens.push_back(tok);
}

void Lex::addToken(enum TokenType type)
{
    Token tok = { type, line, "", 0 };
    tokens.push_back(tok);
}

bool Lex::isDigit(char c)
{
    return c >= '0' && c <= '9';
}

bool Lex::isHex(char c)
{
    return (c >= '0' && c <= '9') ||
           (c >= 'a' && c <= 'f') ||
           (c >= 'A' && c <= 'F');
}

bool Lex::isAlpha(char c)
{
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

void Lex::lex()
{
    char c = advance();
    switch (c)
    {
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        addToken(TOKEN_NEWLINE);
        line++;
        break;
    case ';':
        while (peek() != '\n' && !isAtEnd())
            advance();
        break;
    case ',':
    {
        char thang = advance();
        switch (thang)
        {
        case 'X':
        case 'x': addToken(TOKEN_X); break;
        case 'Y':
        case 'y': addToken(TOKEN_Y); break;
        default:
            std::cerr << "Expected x or y" << std::endl;
            break;
        }
        break;
    }
    case '#':
        addToken(TOKEN_IMM);
        break;
    case '(':
        addToken(TOKEN_OPEN_PAREN);
        break;
    case ')':
        addToken(TOKEN_CLOSE_PAREN);
        break;
    default:
        if (isAlpha(c))
        {
            while (peek() != ' ' && peek() != '\n' && !isAtEnd())
                advance();

            std::string str = text.substr(start, current - start);

            if (std::find(std::begin(instructions), std::end(instructions), str) != std::end(instructions))
                addTokenInstr(str);
            else
                std::cerr << "invalid instruction cuh: " << str << std::endl;
        }
        else if (isDigit(c) || c == '$')
        {
            int base = 10;

            if (c == '$')
            {
                start++;
                base = 16;
                while (isHex(peek()))
                    advance();
            }
            else
            {
                while (isDigit(peek()))
                    advance();
            }

            std::string str = text.substr(start, current - start);
            unsigned long num = std::strtoul(str.c_str(), nullptr, base);
            if (str.length() > 4  || num > INT16_MAX)
            {
                std::cerr << "Number too big - thats what she said" << std::endl;
            }
            addTokenNum((uint16_t)num, str);
        }
        else
        {
            std::cerr << "invalid syntax cuh: " << c << std::endl;
        }
    }
}


std::vector<Token> Lex::lexAll()
{
    while (!isAtEnd())
    {
        start = current;
        lex();
    }

    return tokens;
}