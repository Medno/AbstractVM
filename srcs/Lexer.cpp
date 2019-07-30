#include "Lexer.hpp"

/*
 * Canonical form
*/
Lexer::Lexer( std::string const & input ) : _stream( input ) {
	this->_allTokens = {
		{ "PUSH", {PUSH, "push"} }, { "POP", {POP, "pop"} },
		{ "DUMP", {DUMP, "dump"} }, { "ASSERT", {ASSERT, "assert"} },
		{ "ADD", {ADD, "add"} }, { "SUB", {SUB, "sub"} },
		{ "MUL", {MUL, "mul"} }, { "DIV", {DIV, "div"} },
		{ "MOD", {MOD, "mod"} }, { "PRINT", {PRINT, "print"} },
		{ "EXIT", {EXIT, "exit"} }, { "O_BRACKET", {O_BRACKET, "("} },
		{ "C_BRACKET", {C_BRACKET, ")"} }, { "INT8", {INT8, "int8"} },
		{ "INT16", {INT16, "int16"} }, { "INT32", {INT32, "int32"} },
		{ "FLOAT", {FLOAT, "float"} }, { "DOUBLE", {DOUBLE, "double"} },
		{ "N", {N, ""} }, { "Z", {Z, ""} },
		{ "\x1B[31mOTHER\033[0m", {OTHER, ""} }
	};
	this->lex();
	return ;
}

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

/*
 * Exceptions class
*/
const char*	Lexer::UnknownInstructionException::what( void ) const throw() {
	return "Unknown instruction";
}


std::vector<std::vector<Lexer::tokens> >	Lexer::getTokens( void ) const {
	return this->_tokens;
}

std::vector<std::string>
Lexer::splitStr(std::string const &str, std::string const & spl) const {
	size_t	nl;
	size_t	pos = 0;
	std::string	substr;
	std::vector<std::string>	line;
	const size_t	len = str.size();

	while ( pos < len ) {
		nl = str.find(spl, pos);
		if ( nl == std::string::npos )
			line.push_back(str.substr(pos, len - pos));
		else if ((substr = str.substr(pos, nl - pos)) != "")
			line.push_back(substr);
		pos = nl + 1 == 0 ? len : nl + 1;
	}
	return ( line );
}

/*
 * First we need to split the stream into a vector of lines,
 * then we remove comments, and we only takes words
*/
std::vector<std::vector<std::string> >	Lexer::filterStream( void ) const {
	size_t	nl;
	std::regex	parenthese("([[:alnum:]]) *\\( *(.*) *\\)");
	std::vector<std::string>	lines;
	std::vector<std::string>	words;
	std::vector<std::vector <std::string> >	splitted;

	lines = splitStr( this->_stream, "\n" );
	for ( auto&& l : lines ) {
		nl = l.find( ";", 0 );
		if ( nl != std::string::npos )
			l.erase( nl, l.size() - nl );
		l = std::regex_replace ( l, parenthese, "$1 ( $2 )" );
		std::fill( words.begin(), words.end(), 0 );
		words = splitStr( l, " " );
		if ( words.size() > 0 )
			splitted.push_back( words );
	}
	return ( splitted );
}

Lexer::tokens	Lexer::createSingleToken( std::string const & it ) const {
	std::regex isInt("-?[[:digit:]]+");
	std::regex isFloat("-?[[:digit:]]+.[[:digit:]]+");

	for (auto&& t : this->_allTokens) {
		if ( !it.compare( t.second.second ) )
			return ( Lexer::tokens(t.second.first, it) );
		else if ( std::regex_match(it, isInt) )
			return ( Lexer::tokens(N, it) );
		else if ( std::regex_match(it, isFloat) )
			return ( Lexer::tokens(Z, it) );
	}
	throw UnknownInstructionException();
}

void	Lexer::tokenize( std::vector<std::vector<std::string> > const & lines ) {
	Lexer::tokens	newToken;

	for( auto&& l : lines ) {
		std::vector<tokens>	tokenLine;
//		std::cout << "Treating '" << &l-&lines[0] << "' line..." << std::endl;
		for(auto&& words : l) {
//			std::cout << "Treating '" << words << "' word..." << std::endl;
			try {
				newToken = this->createSingleToken( words );
				tokenLine.push_back( newToken );
			} catch ( UnknownInstructionException & e ) {
				std::cout << "Error: Line " << &l-&lines[0] + 1 << ": "
					<< e.what() << std::endl;
			}
		}
		if ( tokenLine.size() )
		this->_tokens.push_back(tokenLine);
	}
}

//Analyze lines by lines
void	Lexer::lex( void ) {
	std::vector<std::vector<std::string> >	lines = this->filterStream();

	this->tokenize( lines );
	std::cout << *this;

	return ;
}

std::ostream &	operator<<( std::ostream & o, Lexer const & rhs ) {
	std::vector< std::vector<Lexer::tokens > >	tokens = rhs.getTokens();
	for ( auto&& lines : tokens ) {
		o << "Line : " << &lines-&tokens[0] + 1 << std::endl;
		for ( auto&& token : lines ) {
			o << "Token :\t" << rhs._allTokens[token.first].first
			<< "\tValue:\t" << token.second << "\tToken Value : \t" << token.first << std::endl;
		}
	}
	return (o);
}
