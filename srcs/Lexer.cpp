#include "Lexer.hpp"

Lexer::Lexer( std::string const & input ) : _stream( input ) {}

Lexer::~Lexer( void ) {}

Lexer::Lexer( Lexer const & src ) {
	*this = src;
	return ;
}

Lexer	& Lexer::operator=( Lexer const & rhs ) {
	if ( this != &rhs )
		this->_stream = rhs._stream;
	return *this;
}
