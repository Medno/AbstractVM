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
Parser::Parser( Lexer const & lexer ) : error(lexer.getError()) {
	this->registerTokenHandling( PUSH, &Parser::handlePairInstruction );
	this->registerTokenHandling( POP, &Parser::handleSingleInstruction );
	this->registerTokenHandling( DUMP, &Parser::handleSingleInstruction );
	this->registerTokenHandling( ASSERT, &Parser::handlePairInstruction );
	this->registerTokenHandling( ADD, &Parser::handleSingleInstruction );
	this->registerTokenHandling( SUB, &Parser::handleSingleInstruction );
	this->registerTokenHandling( MUL, &Parser::handleSingleInstruction );
	this->registerTokenHandling( DIV, &Parser::handleSingleInstruction );
	this->registerTokenHandling( PRINT, &Parser::handleSingleInstruction );
	this->registerTokenHandling( EXIT, &Parser::handleSingleInstruction );

	this->registerTokenHandling( AND, &Parser::handleSingleInstruction );
	this->registerTokenHandling( OR, &Parser::handleSingleInstruction );
	this->registerTokenHandling( XOR, &Parser::handleSingleInstruction );
	this->registerTokenHandling( NOT, &Parser::handleSingleInstruction );
	this->registerTokenHandling( MIN, &Parser::handleSingleInstruction );
	this->registerTokenHandling( MAX, &Parser::handleSingleInstruction );
	this->registerTokenHandling( OTHER, &Parser::invalidInstruction );
	this->parse(lexer);
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
	this->error += 1;
	try {
		std::cout << "\033[1;31mError:\033[0m \033[1;37mLine " << index + 1 << ": ";
		throw;
	} catch ( std::exception & e ) {
		std::cout << e.what();
	}
	std::cout << "\033[0m" << std::endl;
}

int	Parser::getError( void ) const {
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

void	Parser::registerTokenHandling( tokenLabel label ,memberPtr ptr ) {
	this->handler[label] = ptr;
}

void	Parser::parse( Lexer const & lexer ) {
	std::vector<std::vector<Lexer::token > >	tokens = lexer.getTokens();
	for (auto&& lines : tokens) {
		try {
			if (this->handler.find(lines.front().first) == this->handler.end())
				this->invalidInstruction(lines);
			else if (lines.front().first != OTHER)
				(this->*handler[lines.front().first])(lines);
		} catch ( ... ) {
			handleException( &lines - &tokens[0] );
		}
	}
	return ;
}
