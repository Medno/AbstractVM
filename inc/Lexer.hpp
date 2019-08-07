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
	PUSH = 0, POP, DUMP, ASSERT,
	ADD, SUB, MUL, DIV, MOD,
	PRINT, EXIT, O_BRACKET, C_BRACKET,
	INT8, INT16, INT32, FLOAT, DOUBLE, N, Z, OTHER,
	AND, OR, XOR, NOT
};

class Lexer {
public:
	~Lexer( void );
	Lexer( Lexer const & );
	Lexer	& operator=( Lexer const & );

	Lexer( std::string const &, Options const & );

	typedef std::pair<tokenLabel, std::string> token;
	std::map<std::string, tokenLabel>	allTokens;

	int	getError( void ) const;
	std::vector<std::vector<token > >	getTokens( void ) const;

	class	UnknownInstructionException: public std::exception {
		public:
			virtual const char*	what( void ) const throw();
	};

private:
	Lexer( void );
	void	registerToken( tokenLabel, std::string const & );
	void	lex( Options const & );
	token	createSingleToken( std::string const & ) const;
	void	tokenize( std::vector<std::vector<std::string> > const & );
	std::vector<std::string>	splitStr( std::string const &, std::string const & ) const;
	std::vector<std::vector<std::string> >	filterStream( void ) const;
	void	displayErrors( void );

	std::string	stream;
	std::vector<std::vector<token > >	tokens;
	int 	error;
};

std::ostream &	operator<<( std::ostream &, Lexer const & );

#endif
