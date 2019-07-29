#include "Parser.hpp"

Parser::Parser( Lexer const & lexer ) {
	std::cout << "--------------------- Start Parsing... ---------------------"
		<< std::endl;
	this->_parse(lexer);
}
Parser::~Parser( void ) {}
Parser::Parser( Parser const & src ) {
	*this = src;
	return;
}
Parser	& Parser::operator=( Parser const & ) {
	return *this;
}
/*
 * Exceptions classes
*/

const char*	Parser::IntructionTooLongException::what( void ) const throw() {
	return "This instruction doesn't take an argument";
}

void	Parser::handleException( int const & index ) {
	try {
		std::cout << "Error: Line " << index << ": ";
		throw;
	} catch ( IntructionTooLongException & e ) {
		std::cout << e.what() << std::endl;
	}
}

void	Parser::handleSingleInstruction( std::vector<Lexer::tokens> const & instr) {
	if ( instr.size() > 1 )
		throw IntructionTooLongException(); // TODO: Handle error
}
void	Parser::notYet( std::vector<Lexer::tokens> const & instr) {
	(void)instr;
	std::cout << "Functionality not yet implemented" << std::endl;
}


void	Parser::_parse( Lexer const & lexer ) {
	std::vector<std::vector<Lexer::tokens > >	tokens = lexer.getTokens();
	std::vector<int>	matcher = {
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};
	void	(Parser::*handler[2])( std::vector<Lexer::tokens> const & ) = {
		&Parser::handleSingleInstruction,
		&Parser::notYet
	};

	for (auto&& lines : tokens) {
		try {
			(this->*handler[matcher[lines.front().first]])(lines);
		} catch ( ... ) {
			handleException( &lines - &tokens[0] );
		}
//		std::cout << token.second << std::endl;
	}
	return ;
}
