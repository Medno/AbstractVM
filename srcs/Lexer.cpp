#include "Lexer.hpp"

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
		{ "\x1B[31mOTHER\033[0m", {OTHER, ""} }, { "N", {N, ""} }, { "Z", {Z, ""} }
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

void	Lexer::print( void ) {
	for ( auto&& lines : this->_tokens ) {
		std::cout << "Line : " << &lines-&this->_tokens[0] + 1 << std::endl;
		for ( auto&& token : lines ) {
			std::cout << "Token :\t" << this->_allTokens[token.first].first
			<< "\tValue:\t" << token.second << std::endl;
		}
	}
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

std::vector<std::vector<std::string> >	Lexer::splitStream( void ) const {
	std::vector<std::string>	lines;
	std::vector<std::string>	words;
	std::vector<std::vector <std::string> >	splitted;
	std::regex	oParenthese("([[:alnum:]]) *\\( *(.*) *\\)");
//	std::regex	cParenthese("([[:digit:]])\\)");
//i[a-z]\(( +)?([0-9]+)?( +)?\)
	lines = splitStr(this->_stream, "\n");
	for (auto&& l : lines) {
		l = std::regex_replace (l,oParenthese,"$1 ( $2 )");
//		l = std::regex_replace (l,cParenthese,"$1 )");
		std::fill(words.begin(), words.end(), 0);
		words = splitStr(l, " ");
		splitted.push_back(words);
	}
	return ( splitted );
}

Lexer::tokens	Lexer::createSingleToken( std::string const & it ) const {
	std::regex isInt("-?[[:digit:]]+");
	std::regex isFloat("-?[[:digit:]]+.[[:digit:]]+");

	for (auto&& t : this->_allTokens) {
		if (!it.compare(t.second.second))
			return ( Lexer::tokens(t.second.first, it) );
		else if ( std::regex_match(it, isInt) )
			return ( Lexer::tokens(N, it) );
		else if ( std::regex_match(it, isFloat) )
			return ( Lexer::tokens(Z, it) );
	}
	return ( Lexer::tokens(OTHER, it) );
}

void	Lexer::tokenize( std::vector<std::vector<std::string> > const & lines ) {

	for(auto&& l : lines) {
		std::vector<tokens>	tokenLine;
//		std::cout << "Treating '" << &l-&lines[0] << "' line..." << std::endl;
		for(auto&& words : l) {
//			std::cout << "Treating '" << words << "' word..." << std::endl;
			tokenLine.push_back(this->createSingleToken(words));
		}
		this->_tokens.push_back(tokenLine);
	}
}

//Analyze lines by lines
void	Lexer::lex( void ) {
	std::vector<std::vector<std::string> >	lines = this->splitStream();

	this->tokenize(lines);
	this->print();

	return ;
}
