#ifndef LEXER_HPP
#define LEXER_HPP

#include <sstream>
#include <string>

class Lexer {
public:
	Lexer( std::string const & );
	~Lexer( void );
	Lexer( Lexer const & );
	Lexer	& operator=( Lexer const & );

private:
	std::string	_stream;
	
};

#endif
