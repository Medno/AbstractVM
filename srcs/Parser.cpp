#include "Parser.hpp"

/*
 * Canonical form
*/
Parser::Parser( void ) {}
Parser::~Parser( void ) {}
Parser::Parser( Parser const & src ) {
	*this = src;
	return;
}
Parser	& Parser::operator=( Parser const & rhs ) {
	if ( this != &rhs ) {
		this->error = rhs.error;
	}
	return *this;
}
//	std::cout << "--------------------- Start Parsing... ---------------------"
//		<< std::endl;
	this->_parse(lexer);
Parser::Parser( Lexer const & lexer ) : error(lexer.getError()) {
}

/*
 * Exceptions classes
*/

const char*	Parser::IntructionTooLongException::what( void ) const throw() {
	return "This instruction doesn't take an argument";
}

const char*	Parser::WrongNumberOfArgumentsException::what( void ) const throw() {
	return "This instruction takes a VALUE argument";
}

const char*	Parser::InvalidNumberOfInstructionException::what( void ) const throw() {
	return "Invalid syntax of the VALUE argument";
}

const char*	Parser::OpenBracketMissingException::what( void ) const throw() {
	return "Open bracket is missing for this instruction";
}

const char*	Parser::ClosingBracketMissingException::what( void ) const throw() {
	return "Closing bracket is missing for this instruction";
}

const char*	Parser::InvalidNumberValueZException::what( void ) const throw() {
	return "This instruction takes a FLOATING / DOUBLE argument";
}

const char*	Parser::InvalidNumberValueNException::what( void ) const throw() {
	return "This instruction takes a INT{8,16,32} argument";
}

const char*	Parser::InvalidInstructionException::what( void ) const throw() {
	return "Invalid instruction";
}

void	Parser::handleException( int const & index ) {
	this->error |= 1 << 1;
	try {
		std::cout << "\033[1;31mError\033[0m: Line " << index + 1 << ": ";
		throw;
	} catch ( std::exception & e ) {
		std::cout << e.what() << std::endl;
	}
}

bool	Parser::getError( void ) const {
	return ( this->error );
}
/*
 * Handling instructions
*/

void	Parser::handleSingleInstruction( std::vector<Lexer::token> const & instr) {
	if ( instr.size() > 1 )
		throw IntructionTooLongException();
}

void	Parser::handleValueInstruction( std::vector<Lexer::token> const & instr) {
	if ( instr[2].first != O_BRACKET )
		throw OpenBracketMissingException();
	else if ( instr[4].first != C_BRACKET )
		throw ClosingBracketMissingException();
	if ( ( instr[1].first == DOUBLE || instr[1].first == FLOAT ) && instr[3].first != Z )
		throw InvalidNumberValueZException();
	else if ( ( instr[1].first != DOUBLE && instr[1].first != FLOAT ) && instr[3].first != N)
		throw InvalidNumberValueNException();
}

void	Parser::handlePairInstruction( std::vector<Lexer::token> const & instr) {
	if ( instr.size() != 5 )
		throw WrongNumberOfArgumentsException();
	try {
		this->handleValueInstruction(instr);
	} catch (...) {
		throw;
	}
}

void	Parser::invalidInstruction( std::vector<Lexer::token> const & instr ) {
	if ( instr[0].first != OTHER )
		throw InvalidInstructionException();
}
// End of Error

	std::vector<int>	matcher = {
		1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
	};
	void	(Parser::*handler[3])( std::vector<Lexer::tokens> const & ) = {
		&Parser::handleSingleInstruction,
		&Parser::handlePairInstruction,
		&Parser::invalidInstruction,
	};

void	Parser::parse( Lexer const & lexer ) {
	std::vector<std::vector<Lexer::token > >	tokens = lexer.getTokens();
	for (auto&& lines : tokens) {
		try {
			(this->*handler[matcher[lines.front().first]])(lines);
		} catch ( ... ) {
			handleException( &lines - &tokens[0] );
		}
	}
	return ;
}
