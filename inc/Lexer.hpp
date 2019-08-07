#ifndef LEXER_HPP
#define LEXER_HPP

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <algorithm>

#include "Options.hpp"

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
	N,
	Z,
	OTHER
};

class Lexer {
public:
	~Lexer( void );
	Lexer( Lexer const & );
	Lexer	& operator=( Lexer const & );

	Lexer( std::string const &, Options const & );

	typedef std::pair<tokenLabel, std::string> tokens;

	bool	getError( void ) const;
	std::vector<std::vector<tokens > >	getTokens( void ) const;
	std::map<std::string, tokenLabel>	allTokens;

	class	UnknownInstructionException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};

private:
	Lexer( void );
	void	registerToken( tokenLabel, std::string const & );
	void	lex( Options const & );
	tokens	createSingleToken( std::string const & ) const;
	void	tokenize( std::vector<std::vector<std::string> > const & );
	std::vector<std::string>			splitStr( std::string const &, std::string const & ) const;
	std::vector<std::vector<std::string> >	filterStream( void ) const;

	std::string	_stream;
	std::vector<std::vector<tokens > >	_tokens;
	bool	_error;
};

std::ostream &	operator<<( std::ostream &, Lexer const & );

#endif
