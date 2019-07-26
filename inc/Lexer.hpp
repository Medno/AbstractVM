#ifndef LEXER_HPP
#define LEXER_HPP

#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <regex>

enum	tokenLabel {
	PUSH = 0,
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
	O_BRACKET,
	C_BRACKET,
	INT8,
	INT16,
	INT32,
	FLOAT,
	DOUBLE,
	OTHER,
	N,
	Z,
	VALUE
};

class Lexer {
public:
	Lexer( std::string const & );
	~Lexer( void );
	Lexer( Lexer const & );
	Lexer	& operator=( Lexer const & );

	std::vector<std::vector<std::string> >	splitStream( void ) const;
	void	tokenize( std::vector<std::vector<std::string> > const & );
	void	lexLine( std::string const & );
	void	lex( void );

	typedef std::pair<tokenLabel, std::string> tokens;

private:
	std::vector<std::string>	splitStr( std::string const &, std::string const & ) const;
	tokens	createSingleToken( std::string const & ) const;

	std::string	_stream;
	std::vector<std::vector<tokens > >	_tokens;
	void	print( void );
	std::vector<std::pair<std::string, tokens > >	_allTokens;
};

#endif
