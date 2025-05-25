#pragma once

#include <string>
#include <vector>

enum TokenType
{
	TOKEN_INSTR,
	TOKEN_8BIT,
	TOKEN_16BIT,
	TOKEN_X,
	TOKEN_Y,
	TOKEN_IMM,
	TOKEN_OPEN_PAREN,
	TOKEN_CLOSE_PAREN,
	TOKEN_NEWLINE,
};

struct Token
{
	enum TokenType type;
	int line;
	std::string string;
	uint16_t num;
};

class Lex
{
private:
	int line;
	int start;
	int current;
	int instrNum;
	std::string text;
	std::vector<Token> tokens;
public:
	Lex(std::string text);
	std::vector<Token> lexAll();

	int getInstrNum()
	{
		return instrNum;
	}
private:
	bool isAtEnd();
	char advance();
	bool match(char c);
	char peek();
	char peekNext();
	void addTokenInstr(std::string name);
	void addTokenNum(uint16_t num, std::string str);
	void addToken(enum TokenType type);
	bool isDigit(char c);
	bool isHex(char c);
	bool isAlpha(char c);
	void lex();
};