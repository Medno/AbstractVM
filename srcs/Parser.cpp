#include "Parser.hpp"

Parser::Parser( void ) {}
Parser::~Parser( void ) {}
Parser::Parser( Parser const & src ) {
	*this = src;
	return;
}
Parser	& Parser::operator=( Parser const & rhs ) {
	return *this;
}
