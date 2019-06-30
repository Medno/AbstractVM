#ifndef LEXER_HPP
#define LEXER_HPP

#include <sstream>
#include <string>
#include <vector>
#include <iostream>

enum	tokenLabel {
	PUSH,
	POP,
	DUMP,
	ASSERT,
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	PRINT,
	EXIT,
	OPERATOR,
	O_BRACKET,
	C_BRACKET,
	VALUE,
	OTHER
};

class Lexer {
public:
	Lexer( std::string const & );
	~Lexer( void );
	Lexer( Lexer const & );
	Lexer	& operator=( Lexer const & );

	std::vector<std::string>	split( void );
	void	lexLine( std::string const & );
	void	lex( void );

private:
	std::string	_stream;
	std::vector<std::pair<tokenLabel, std::string> >	_tokens;
	
};

#endif
